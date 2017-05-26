#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "icc.h"
#include <getopt.h>
#include <time.h>
#include <unistd.h>


/* Do mmap to cover entire BW L1 range from 0x24_0000 to 0x3A_0000 */
#define SHARC_L1_SIZE  (1408 * 1024)
#define SHARC_L1_START 0x240000

/* Calculate beginning of L1 accessed from MP region */
#define SHARC1_S1_PORT 0x28000000
#define SHARC2_S1_PORT 0x28800000
#define SHARC1_L1_MPS1_START (SHARC1_S1_PORT | SHARC_L1_START)
#define SHARC2_L1_MPS1_START (SHARC2_S1_PORT | SHARC_L1_START)

/* time to wait in polling loop before exitin */
#define WAIT_TIME_SEC 8

/* location of bootrom that loops idle */
#define SHARC_LOOP_IDLE 0x5000a6


extern struct appLdrSect{
  uint32_t tgt;
  uint8_t *sect;
  uint32_t sectsz;
} mySectsSharc1[], mySectsSharc2[];

enum memtype{
  sharcl1 = 0,
  sharedl2
};

/* flag to determine how much debug info to print out */
int dbglvl=0;


void loadSharcApp(struct appLdrSect appSects[], int coren){
  extern uint32_t sh1_numsects, sh2_numsects; 
  uint32_t numsects; 
  int i, j, sectSz;
  uint8_t *finalTgtAddr, *tgt;
  uint8_t *data;
  uint32_t rng, uTgt;
  int memfd;
  void * mappedmem;
  unsigned int sharc_s1_port, sharc_l1_mps1_start;

  if(coren == 1){
    sharc_s1_port       = SHARC1_S1_PORT;
    sharc_l1_mps1_start = SHARC1_L1_MPS1_START;
    numsects = sh1_numsects;
  }
  else if(coren == 2){
    sharc_s1_port       = SHARC2_S1_PORT;
    sharc_l1_mps1_start = SHARC2_L1_MPS1_START;
    numsects = sh2_numsects;
  }
  else{
    if(dbglvl > 0)
      printf("Bad core number passed in!  %d\n", coren);
    return;
  }

  if(dbglvl > 1)
    printf("number of sections: %d\n", numsects);

  for(i=0; i<numsects; i++){
    tgt  = (uint8_t *)(appSects[i].tgt);
    data = appSects[i].sect;
    sectSz = appSects[i].sectsz;

    if(dbglvl > 2)
      printf("i: %d, tgt: %08x, size of data: %d\n", i, (int)tgt, sectSz);

    uTgt = (uint32_t)tgt;
    /* test if sect in L2 */
    /*   this case shouldn't happen.  the "stub" should all be in L1 bank3 */
    rng = (uTgt & 0xffff0000) >> 16;
    if( rng >= 0x2008 && rng < 0x200c){

      memfd = open("/dev/mem", O_RDWR | O_SYNC);
      if(memfd < 0){
	perror("error opening /dev/mem for L2");
	exit(10);
      }
      mappedmem = mmap(0, (256*1024), PROT_READ|PROT_WRITE, MAP_SHARED, memfd, 0x20080000);
      if(mappedmem == MAP_FAILED){
	perror("error mapping L1");
	exit(10);
      }
      
      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem + ((unsigned int)tgt - 0x20080000));
      
      /* copying to L2 */
      memcpy(tgt, data, sectSz);

      munmap(mappedmem, (256*1024));
      close(memfd);

    }


    /* if not L2, then must be L1 for all other address ranges below */
    memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if(memfd < 0){
      perror("error opening /dev/mem for L1");
      exit(10);
    }
    mappedmem = mmap(0, SHARC_L1_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, sharc_l1_mps1_start);
    if(mappedmem == MAP_FAILED){
      perror("error mapping L1");
      exit(10);
    }
    


    /* test if sect in BW */
    if((uTgt >= 0x240000 && uTgt < 0x270000) ||
       (uTgt >= 0x2c0000 && uTgt < 0x2F0000) ||
       (uTgt >= 0x300000 && uTgt < 0x320000) ||
       (uTgt >= 0x380000 && uTgt < 0x3A0000)){

      uTgt = ((uTgt & 0x003fffff) | sharc_s1_port);
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem + ((unsigned int)tgt - sharc_l1_mps1_start));

      /* copying to L1 (BW) */
      memcpy(tgt, data, sectSz);

    }

    /* test if sect in LW */
    if((uTgt >= 0x48000 && uTgt < 0x4e000) ||
       (uTgt >= 0x58000 && uTgt < 0x5e000) ||
       (uTgt >= 0x60000 && uTgt < 0x64000) ||
       (uTgt >= 0x70000 && uTgt < 0x74000)) {
      uTgt = uTgt << 3;                           // convert to BW address
      uTgt = ((uTgt & 0x003fffff) | sharc_s1_port);  // convert to MP address
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem + ((unsigned int)tgt - sharc_l1_mps1_start));


      /* assert size of this section is mult of 8 */
      if((sectSz & 7) != 0)
	if(dbglvl > 2)
	  printf("LW section sz not div by 8: %d\n", sectSz);

      for(j=0; j<sectSz; j+=8){
	uint8_t *d = &tgt[j], *s = &data[j];
	d[0] = s[7];
	d[1] = s[6];
	d[2] = s[5];
	d[3] = s[4];
	d[4] = s[3];
	d[5] = s[2];
	d[6] = s[1];
	d[7] = s[0];
      }

    }


    /* test if sect in NW/EP */
    if((uTgt >= 0x90000 && uTgt < 0x9c000) ||
       (uTgt >= 0xb0000 && uTgt < 0xbc000) ||
       (uTgt >= 0xc0000 && uTgt < 0xc8000) ||
       (uTgt >= 0xe0000 && uTgt < 0xe8000)) {
      uTgt = uTgt << 2;                           // convert to BW address
      uTgt = ((uTgt & 0x003fffff) | sharc_s1_port);  // convert to MP address
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem + ((unsigned int)tgt - sharc_l1_mps1_start));

      /* assert size of this section is mult of 4 */
      if((sectSz & 3) != 0)
	if(dbglvl > 2)
	  printf("NW/EP section sz not div by 4: %d\n", sectSz);

      for(j=0; j<sectSz; j+=4){
	uint8_t *d = &tgt[j], *s = &data[j];
	d[0] = s[3];
	d[1] = s[2];
	d[2] = s[1];
	d[3] = s[0];
      }

    }



    /* test if sect in SW */
    if((uTgt >= 0x120000 && uTgt < 0x138000) ||
       (uTgt >= 0x160000 && uTgt < 0x178000) ||
       (uTgt >= 0x180000 && uTgt < 0x190000) ||
       (uTgt >= 0x1c0000 && uTgt < 0x1d0000)) {
      uTgt = uTgt << 1;                           // convert to BW address
      uTgt = ((uTgt & 0x003fffff) | sharc_s1_port);  // convert to MP address
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem + ((unsigned int)tgt - sharc_l1_mps1_start));

      /* assert size of this section is mult of 2 */
      if((sectSz & 1) != 0)
	if(dbglvl > 2)
	  printf("SW section sz not div by 2: %d\n", sectSz);

      for(j=0; j<sectSz; j+=2){
	uint8_t *d = &tgt[j], *s = &data[j];
	d[0] = s[1];
	d[1] = s[0];

      }

    }


    /* shut down mmap of L1 */
    munmap(mappedmem, SHARC_L1_SIZE);
    close(memfd);
  }

  return;

}


#define USAGE                                                                           \
"usage:\n"                                                                              \
"  loadSharc [options]\n"                                                               \
"options:\n"                                                                            \
"  -i [LDR file]           Sharc Binary LDR file to be \n"                              \
"  -s [Sharc Core #]       Sharc Core to load (1 or 2)\n"                               \
"  -d [debug level]    Amount of debug info 1, 2 or 3\n"                                \
"  -h                  Show this help message\n"                             



/* OPTIONS DESCRIPTOR ================================ */
static struct option gLongOptions[] = {
  {"input", required_argument,  NULL,     'i'},
  {"sharc", required_argument,  NULL,     's'},
  {"debug", required_argument,  NULL,     'd'},
  {"help",  no_argument,        NULL,     'h'},
  {NULL,    0,                  NULL,      0}
};


void Usage() {
  fprintf(stdout, "%s", USAGE);
}

int main(int argc, char *argv[])
{
  int ccfd, ret, filesz;
  extern uint32_t sh1_progStartAddr, sh2_progStartAddr;
  uint32_t progStartAddr;
  FILE *fd;
  int memfd;
  int option_char;
  void *mappedmem, *ddr1mappedmem;
  extern volatile uint32_t *ptrMboxSh2Arm_sh1, *ptrMboxArm2Sh_sh1, *ptrMboxSh2Arm_sh2, *ptrMboxArm2Sh_sh2;
  volatile uint32_t *ptrMboxSh2Arm, *ptrMboxArm2Sh;
  extern uint32_t *ptrBootStream_sh1, *ptrBootStream_sh2;
  uint32_t *ptrBootStream;
  extern volatile uint32_t *ptrLdSync2ArmByteCount_sh1, *ptrLdSync2ShByteCount_sh1, *ptrLdSync2ArmByteCount_sh2, *ptrLdSync2ShByteCount_sh2;
  volatile uint32_t *ptrLdSync2ArmByteCount, *ptrLdSync2ShByteCount;
  int reqSz, rdSz;
  int blockNum = 0;
  int flag = 0;
  time_t startt;
  int sharc_core=1;       // default to sharc core 1   
  unsigned long sharc_s1_port, sharc_l1_mps1_start;

  if(argc < 2){
    printf("Nothing to be done...exiting\n");
    goto _exit; 
  } 

  while ((option_char = getopt_long(argc, argv, "i:s:d:h", gLongOptions, NULL)) != -1) {
    switch (option_char) {
    case 'i': // input file
      printf("reading %s\n", optarg);
      fd = fopen(optarg, "rb");
      if(fd == NULL){
	printf("problem opening %s\n", optarg);
	goto _exit;
      }
      break;  
    case 's': // which sharc core
      sharc_core = atoi(optarg);
      if(sharc_core != 1 && sharc_core != 2){
	printf("invalid sharc core to load: %d\n", sharc_core);
	goto _exit;
      }
      break;
    case 'd': // debug level
      dbglvl = atoi(optarg);
      if(dbglvl < 1 || dbglvl > 3)
	dbglvl = 0;          // reset if set out of range 

      printf("DEBUG LVL: %d\n", dbglvl);
      break;
    case 'h': // help
      Usage();
      goto _exit;
      break;   
    default:
      Usage();
      goto _exit;
    }
  }

  if(sharc_core == 1){
    sharc_s1_port       = SHARC1_S1_PORT;
    sharc_l1_mps1_start = SHARC1_L1_MPS1_START;
    progStartAddr       = sh1_progStartAddr;
    ptrMboxSh2Arm       = ptrMboxSh2Arm_sh1;
    ptrMboxArm2Sh       = ptrMboxArm2Sh_sh1;
    ptrBootStream       = ptrBootStream_sh1;
    ptrLdSync2ArmByteCount = ptrLdSync2ArmByteCount_sh1;
    ptrLdSync2ShByteCount  = ptrLdSync2ShByteCount_sh1;
  }else if(sharc_core == 2){
    sharc_s1_port       = SHARC2_S1_PORT;
    sharc_l1_mps1_start = SHARC2_L1_MPS1_START;
    progStartAddr       = sh1_progStartAddr;
    ptrMboxSh2Arm       = ptrMboxSh2Arm_sh2;
    ptrMboxArm2Sh       = ptrMboxArm2Sh_sh2;
    ptrBootStream       = ptrBootStream_sh2;
    ptrLdSync2ArmByteCount = ptrLdSync2ArmByteCount_sh2;
    ptrLdSync2ShByteCount  = ptrLdSync2ShByteCount_sh2;
  }else{
    if(dbglvl > 0)
      printf("Something is wrong... sharc_core = %d\n", sharc_core);
    goto _exit;
  }




  if(dbglvl > 0)
    printf("booting sharc core %d\n", sharc_core);

  /* open corecontrol */
  ccfd = open("/dev/corectrl", O_RDWR);
  if(ccfd < 0){
    if(dbglvl > 0)
      printf("unable to open /dev/corectrl");
    goto _exit;
  }

  /* set svect[n] to spin on IDLE in boot rom */
  ret = ioctl(ccfd, CMD_SET_SVECT1, SHARC_LOOP_IDLE);

  if(ret < 0){
    if(dbglvl > 0)
      printf("unable to set svect%d\n", sharc_core);
    goto _exit;
  }

  /* reset core n so it spins on jump idle */
  ret = ioctl(ccfd, CMD_CORE_START, sharc_core);
  if(ret < 0){
    if(dbglvl > 0)
      printf("unable to start core %d to spin on idle in boot rom", sharc_core);
    goto _exit;
  }


  /* copy (load) sharc app */
  loadSharcApp((sharc_core == 1) ? mySectsSharc1 : mySectsSharc2, sharc_core);



  /* set svect1 to beginning of app */
  ret = ioctl(ccfd, (sharc_core==1) ? CMD_SET_SVECT1 : CMD_SET_SVECT2, progStartAddr);
  if(ret < 0){
    if(dbglvl > 0)
      printf("unable to set svect%d to start of app", sharc_core);
    goto _exit;
  }
  
  /* reset core 1 so it vectors to start of app */
  ret = ioctl(ccfd, CMD_CORE_START, sharc_core);
  if(ret < 0){
    if(dbglvl > 0)
      printf("unable to start core %d to start of app", sharc_core);
    goto _exit;
  }


  /* map for maibox vars */
  memfd = open("/dev/mem", O_RDWR | O_SYNC);
  if(memfd < 0){
    perror("error opening /dev/mem for L2");
    goto _exit;
  }

  mappedmem = mmap(0, SHARC_L1_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, sharc_l1_mps1_start);
  if(mappedmem == MAP_FAILED){
    perror("error mapping L1");
    goto _cleanup_closemap;
  }

  if(dbglvl > 2){
    printf("ptrMboxSh2Arm = %x\n", (uint32_t)ptrMboxSh2Arm);
    printf("ptrMboxArm2Sh = %x\n", (uint32_t)ptrMboxArm2Sh);
  }

  /* convert to MP address */
  ptrMboxSh2Arm = (uint32_t *)(((uint32_t)ptrMboxSh2Arm & 0x003fffff) | sharc_s1_port);
  ptrMboxArm2Sh = (uint32_t *)(((uint32_t)ptrMboxArm2Sh & 0x003fffff) | sharc_s1_port);

  if(dbglvl > 2){
    printf("ptrMboxSh2Arm = %x\n", (uint32_t)ptrMboxSh2Arm);
    printf("ptrMboxArm2Sh = %x\n", (uint32_t)ptrMboxArm2Sh);
  }

  /* calc mapped address for mbox vars */
  ptrMboxSh2Arm = (uint32_t *)((uint32_t)mappedmem + ((uint32_t)ptrMboxSh2Arm - sharc_l1_mps1_start));
  ptrMboxArm2Sh = (uint32_t *)((uint32_t)mappedmem + ((uint32_t)ptrMboxArm2Sh - sharc_l1_mps1_start));


  /* Signal to Sharc that data is *not* ready */
  *ptrMboxArm2Sh = 0xdead;

  /* wait for sharc to start executing */
  if(dbglvl > 1)
    printf("Waiting for sharc\n");
  time(&startt);
  while(*ptrMboxSh2Arm != 0xb0b0)
    if(difftime(time(NULL), startt) > WAIT_TIME_SEC){       // exit if we wait too long
      if(dbglvl > 0)
	printf("time expired waiting for Sharc msg...exiting\n");
      goto _cleanup;
    }

  
  if(dbglvl > 2){
    printf("ptrLdSync2ArmByteCount = %x\n", (uint32_t)ptrLdSync2ArmByteCount);
    printf("ptrLdSync2ShByteCount  = %x\n", (uint32_t)ptrLdSync2ShByteCount);
    printf("ptrBootStream          = %x\n", (uint32_t)ptrBootStream);
  }

  /* convert to MP address */
  ptrLdSync2ArmByteCount = (uint32_t *)(((uint32_t)ptrLdSync2ArmByteCount & 0x003fffff) | sharc_s1_port);
  ptrLdSync2ShByteCount   = (uint32_t *)(((uint32_t)ptrLdSync2ShByteCount   & 0x003fffff) | sharc_s1_port);
  ptrBootStream                    = (uint32_t *)(((uint32_t)ptrBootStream                   & 0x003fffff) | sharc_s1_port);

  if(dbglvl > 2){
    printf("ptrLdSync2ArmByteCount = %x\n", (uint32_t)ptrLdSync2ArmByteCount);
    printf("ptrLdSync2ShByteCount  = %x\n", (uint32_t)ptrLdSync2ShByteCount);
    printf("ptrBootStream          = %x\n", (uint32_t)ptrBootStream);
  }

  /* calc mapped address for mbox vars */
  ptrLdSync2ArmByteCount = (uint32_t *)((uint32_t)mappedmem + ((uint32_t)ptrLdSync2ArmByteCount - sharc_l1_mps1_start));
  ptrLdSync2ShByteCount  = (uint32_t *)((uint32_t)mappedmem + ((uint32_t)ptrLdSync2ShByteCount  - sharc_l1_mps1_start));
  ptrBootStream          = (uint32_t *)((uint32_t)mappedmem + ((uint32_t)ptrBootStream          - sharc_l1_mps1_start));

  while(1){


    /* Signal to Sharc "let's go"  */
    *ptrMboxArm2Sh = 0x5555;

    /* wait for Sharc to request data */
    if(dbglvl > 1)
      printf("waiting for sharc to request data for block num %d\n", blockNum);

    flag = 0;

    time(&startt);                                            // initialize timer
    while(*ptrMboxSh2Arm != 0xbbbb){
      if(difftime(time(NULL), startt) > WAIT_TIME_SEC){       // exit if we wait too long
	if(dbglvl > 0)
	  printf("time expired waiting for Sharc msg...exiting\n");
	goto _cleanup;
      }

      if(flag == 0){
	if(dbglvl > 2)
	   printf("*ptrMboxSh2Arm = %x\n", *ptrMboxSh2Arm);
	flag = 1;
      }
	 
    }



    /* got requested byte count from Sharc - ARM's turn */
    *ptrMboxArm2Sh = 0xbbbb;



    /* ldSync2ArmByteCount should have requested byte count */
    if(dbglvl > 1)
      printf("requested bytes: %d\n", *ptrLdSync2ArmByteCount);
    reqSz = *ptrLdSync2ArmByteCount;

    if(reqSz == 0){
      if(dbglvl > 0)
	printf("Sharc requested 0 more bytes... exiting\n");
      break;
    }
    if(reqSz == -1){
      if(dbglvl > 0)
	printf("Sharc signaled to stop... exiting\n");
      break;
    }


    rdSz = fread(ptrBootStream, 1, reqSz, fd);

    if(rdSz != reqSz){
      if(dbglvl > 0){
	printf("read in %d but requested %d!\n", rdSz, reqSz);
	printf("exiting\n");
      }
      break;
    }

    blockNum ++;

    *ptrLdSync2ShByteCount = rdSz;
    
    /* Tell Sharc that data is ready */
    *ptrMboxArm2Sh = 0x6666;

    /* wait for ack */
    time(&startt);
    while(*ptrMboxSh2Arm != 0x6666)
      if(difftime(time(NULL), startt) > WAIT_TIME_SEC){       // exit if we wait too long
	if(dbglvl > 0)
	  printf("time expired waiting for Sharc msg...exiting\n");
	goto _cleanup;
      }
  }
  
  
  /* clean up */
_cleanup:
_cleanup_unmap:
  munmap(mappedmem, SHARC_L1_SIZE);
_cleanup_closemap:
  close(memfd);

_exit:
  return 0;

}
