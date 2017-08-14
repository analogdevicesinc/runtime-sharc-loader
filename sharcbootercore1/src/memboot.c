#include <cdefSC58x_rom.h>
#include <services/pwr/adi_pwr.h>

//#define READ_FROM_EXTMEM 1
//#define READ_FULL_FILE 1

#if defined (READ_FULL_FILE)
#if defined(READ_FROM_EXTMEM)
/* define a space where we can store the entire boot stream */
#define DDR1_START (0xC0000000)
uint8_t *bs = (uint8_t *)(DDR1_START);   // this address needs to be agreed upon with loader.c (linux)
#else
/* place to store bootstream --- test code.  */
extern uint8_t bootstream[];
extern uint32_t bsSize;
uint8_t *bs = bootstream;
#endif // READ_FROM_EXTMEM
#else
/* read pieces of the boot stream at a time */
uint32_t bootstream[(2*1024)/4];
uint8_t *bs = (uint8_t *)bootstream;
#endif // READ_FULL_FILE

ADI_ROM_BOOT_CONFIG *pBootStructPointer;
ADI_ROM_BOOT_CONFIG bootConfig;


volatile uint32_t mailbox2arm;
volatile uint32_t mailbox2sharc;

volatile uint32_t ldSync2ArmByteCount;
volatile uint32_t ldSync2ShByteCount;


/*******************************************************************************/
/*                                                                             */
/*  Load Routine                                                               */
/*   Just doing a memcopy                                                      */
/*   Note:  we can try to use DMA but might be competing against other cores   */
/*          for its use                                                        */
/*******************************************************************************/
/*!
* @brief      Memory Boot Mode Load Routine
*
* @param [in]  pBootConfig      Pointer to the #ADI_ROM_BOOT_CONFIG structure
*
* @return ROM_BOOT_RESULT
*    -  #ROM_BMODE_SUCCESS
*
* @details
*/
ROM_BOOT_RESULT mboot_load(ADI_ROM_BOOT_CONFIG * pBootConfig)
{
  /* main part of runtime load from memory */
  int i;
  uint8_t *src, *dst;

#ifdef READ_FULL_FILE
  src = pBootConfig->pSource;
#else
  src = (uint8_t *)bs;             // expect ARM to reload to same mem location


  while(mailbox2sharc != 0x5555)   // wait for Sharc's turn
	  ;
  mailbox2arm = 0x5555;            // tell Arm that Sharc is working

  ldSync2ArmByteCount = pBootConfig->dByteCount;  // tell ARM how much data to fread

  mailbox2arm   = 0xbbbb;          // signal to ARM to put data into buffer

  while(mailbox2sharc != 0x6666)   // wait for ARM to fetch data from file
	  ;
  mailbox2arm = 0x6666;            // tell Arm that Sharc is working

   /* the fread byte count returned from ARM to be the same as what was requested */
  if(ldSync2ShByteCount != pBootConfig->dByteCount){
	  mailbox2arm = 0xdead;
	  return ROM_BOOT_FAILURE;
  }

#endif
  dst = pBootConfig->pDestination;


  /* just byte-copy from boot source to memory */
  for (i=0; i<pBootConfig->dByteCount; i++)
    *dst++ = *src++;

  /* Update pSource pointer */
  pBootConfig->pSource = (void *) ((uint32_t) pBootConfig->pSource + (pBootConfig->uwSrcModifyMult * pBootConfig->dByteCount));

  return ROM_BOOT_SUCCESS;
}


/************************************************************************/
/*                                                                      */
/* main()                                                               */    
/*                                                                      */
/************************************************************************/

main()
{
  int i;
  uint8_t *t;

  ROM_BOOT_RESULT result = ROM_BOOT_FAILURE;

  void (*my_prog)(void);

  ADI_ROM_BOOT_HEADER blockHeaderC0;

  mailbox2arm =0xb0b0;   // tell linux app is ready

  while(mailbox2sharc != 0x5555)
	  ;

  pBootStructPointer       = &bootConfig;
  bootConfig.errorReturn   = 0;
  bootConfig.pSource       = bs;

#if 0 // test if it will work booting from L3  (or we can fill mem from from file in cces)
  t = (uint8_t *)0xc0000000;
  for(i=0; i<bsSize; i++)
	  *t++ = bootstream[i];
  bootConfig.pSource       = 0xc0000000;
#endif


  bootConfig.dBootCommand  = 0;  /*command; */
  bootConfig.dFlags        = 0;  /* no need to pass any global flags; //flags; */
  bootConfig.ulBlockCount  = 0;  /* Have always passed this as 0 ; //blockCount; */
  bootConfig.pHeader       = &blockHeaderC0;
  bootConfig.pLogBuffer    = 0;
  bootConfig.pLogCurrent   = 0;
  bootConfig.dLogByteCount = 0;
  bootConfig.pForwardFunction = 0;
  bootConfig.pCrcFunction  = 0;
  bootConfig.pErrorFunction = 0;
  bootConfig.uwDataWidth   = 0;
  bootConfig.uwUserShort   = 0;
  bootConfig.dUserLong     = 0;
  bootConfig.dReserved     = 0;
  bootConfig.pControlRegister = 0;
  bootConfig.uwDstModifyMult = 1;
  bootConfig.uwSrcModifyMult = 1;
  bootConfig.pDmaBaseRegister =0;
  bootConfig.uwDataWidth   = 0;
  bootConfig.pDestination  = 0;                        /*! Destination Pointer used for receive operations*/
  bootConfig.dByteCount    = 0;                        /*! Byte Count */
  bootConfig.loadType=ROM_DMA_DONE_POLL_IRQDONE;       /*! DMA Done Detection Method */

  /*Register the load function. */
  bootConfig.bootRegistry.pLoadFunction= mboot_load;


  /* bootrom uses mdma1 - make secure in case any memory is secure */
  *pREG_SPU0_SECUREP90 = 3;
  *pREG_SPU0_SECUREP91 = 3;

  adi_rom_BootKernel(pBootStructPointer);

  mailbox2arm = 0x5555;            // tell Arm that Sharc is working
  ldSync2ArmByteCount = -1;        // tell ARM that no more data is needed
  mailbox2arm   = 0xbbbb;          // signal to ARM to put data into buffer


  /*call the application entry point which is stored in the RCU_SVECT[n] registers and Release other cores.*/
  my_prog= *pREG_RCU0_SVECT1;
  my_prog();

}
