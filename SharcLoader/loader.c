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

/* Define to disable the load of L1 sections to avoid parity issues when code is in L2 */
#define DISGARD_L1_SECTION

#ifdef SC589
/* Do mmap to cover entire BW L1 range from 0x24_0000 to 0x3A_0000 */
#define SHARC_L1_SIZE        (1408 * 1024)
#define SHARC_L1_START       0x240000

#define SHARC_L1_B1_BW_START 0x00240000
#define SHARC_L1_B1_BW_END   0x00270000
#define SHARC_L1_B2_BW_START 0x002C0000
#define SHARC_L1_B2_BW_END   0x002F0000
#define SHARC_L1_B3_BW_START 0x00300000
#define SHARC_L1_B3_BW_END   0x00320000
#define SHARC_L1_B4_BW_START 0x00380000
#define SHARC_L1_B4_BW_END   0x003A0000

#define SHARC_L1_B1_LW_START 0x00480000
#define SHARC_L1_B1_LW_END   0x004E0000
#define SHARC_L1_B2_LW_START 0x00580000
#define SHARC_L1_B2_LW_END   0x005E0000
#define SHARC_L1_B3_LW_START 0x00600000
#define SHARC_L1_B3_LW_END   0x00640000
#define SHARC_L1_B4_LW_START 0x00700000
#define SHARC_L1_B4_LW_END   0x00740000

#define SHARC_L1_B1_NW_START 0x00090000
#define SHARC_L1_B1_NW_END   0x0009C000
#define SHARC_L1_B2_NW_START 0x000B0000
#define SHARC_L1_B2_NW_END   0x000BC000
#define SHARC_L1_B3_NW_START 0x000C0000
#define SHARC_L1_B3_NW_END   0x000C8000
#define SHARC_L1_B4_NW_START 0x000E0000
#define SHARC_L1_B4_NW_END   0x000E8000

#define SHARC_L1_B1_SW_START 0x00120000
#define SHARC_L1_B1_SW_END   0x00138000
#define SHARC_L1_B2_SW_START 0x00160000
#define SHARC_L1_B2_SW_END   0x00178000
#define SHARC_L1_B3_SW_START 0x00180000
#define SHARC_L1_B3_SW_END   0x00190000
#define SHARC_L1_B4_SW_START 0x001C0000
#define SHARC_L1_B4_SW_END   0x001D0000

#define SHARC_L2_SIZE        (256 * 1024)

#define SHARC_L2_START       0x20080000
#define SHARC_L2_END         0x200C0000
#define SHARC_L2_NW_START    0x08020000
#define SHARC_L2_NW_END      0x08030000
#define SHARC_L2_VISA_START  0x00BA0000
#define SHARC_L2_VISA_END    0x00BC0000
#define SHARC_L2_ISA_START   0x005A0000
#define SHARC_L2_ISA_END     0x005C0000

#define SHARC_L2_VISA_ISA_OFFSET 0x1E940000
#endif

#ifdef SC573
/* Do mmap to cover entire BW L1 range from 0x24_0000 to 0x39_0000 */
#define SHARC_L1_SIZE        (1344 * 1024)
#define SHARC_L1_START       0x240000

#define SHARC_L1_B1_BW_START 0x00240000
#define SHARC_L1_B1_BW_END   0x00260000
#define SHARC_L1_B2_BW_START 0x002C0000
#define SHARC_L1_B2_BW_END   0x002E0000
#define SHARC_L1_B3_BW_START 0x00300000
#define SHARC_L1_B3_BW_END   0x00310000
#define SHARC_L1_B4_BW_START 0x00380000
#define SHARC_L1_B4_BW_END   0x00390000

#define SHARC_L1_B1_LW_START 0x00480000
#define SHARC_L1_B1_LW_END   0x004C0000
#define SHARC_L1_B2_LW_START 0x00580000
#define SHARC_L1_B2_LW_END   0x005C0000
#define SHARC_L1_B3_LW_START 0x00600000
#define SHARC_L1_B3_LW_END   0x00620000
#define SHARC_L1_B4_LW_START 0x00700000
#define SHARC_L1_B4_LW_END   0x00720000

#define SHARC_L1_B1_NW_START 0x00090000
#define SHARC_L1_B1_NW_END   0x00098000
#define SHARC_L1_B2_NW_START 0x000B0000
#define SHARC_L1_B2_NW_END   0x000B8000
#define SHARC_L1_B3_NW_START 0x000C0000
#define SHARC_L1_B3_NW_END   0x000C4000
#define SHARC_L1_B4_NW_START 0x000E0000
#define SHARC_L1_B4_NW_END   0x000E4000

#define SHARC_L1_B1_SW_START 0x00120000
#define SHARC_L1_B1_SW_END   0x00130000
#define SHARC_L1_B2_SW_START 0x00160000
#define SHARC_L1_B2_SW_END   0x00170000
#define SHARC_L1_B3_SW_START 0x00180000
#define SHARC_L1_B3_SW_END   0x00188000
#define SHARC_L1_B4_SW_START 0x001C0000
#define SHARC_L1_B4_SW_END   0x001C8000

#define SHARC_L2_SIZE        (1024 * 1024)

#define SHARC_L2_START       0x20000000
#define SHARC_L2_END         0x20100000
#define SHARC_L2_NW_START    0x08000000
#define SHARC_L2_NW_END      0x08040000
#define SHARC_L2_VISA_START  0x00B80000
#define SHARC_L2_VISA_END    0x00C00000
#define SHARC_L2_ISA_START   0x005C0000
#define SHARC_L2_ISA_END     0x00600000

#define SHARC_L2_VISA_ISA_OFFSET 0x1E900000
#endif

#ifdef SC589
/* Calculate beginning of L1 accessed from MP region */
#define SHARC1_S1_PORT 0x28000000
#define SHARC2_S1_PORT 0x28800000
#define SHARC1_L1_MPS1_START (SHARC1_S1_PORT | SHARC_L1_START)
#define SHARC2_L1_MPS1_START (SHARC2_S1_PORT | SHARC_L1_START)
#endif

#ifdef SC573
/* Calculate beginning of L1 accessed from MP region */
#define SHARC1_S1_PORT 0x28000000
#define SHARC2_S1_PORT 0x28800000
#define SHARC1_L1_MPS1_START (SHARC1_S1_PORT | SHARC_L1_START)
#define SHARC2_L1_MPS1_START (SHARC2_S1_PORT | SHARC_L1_START)
#endif

/* mcapi address is referencing to the start of L2, and is same for both 573 and 589 */
#define SHARC_L2_MCAPI_SH1_OFFSET (12*1024)
#define SHARC_L2_MCAPI_SH2_OFFSET (8*1024)
#define SHARC_L2_MCAPI_SH1_START (SHARC_L2_START + SHARC_L2_MCAPI_SH1_OFFSET)
#define SHARC_L2_MCAPI_SH2_START (SHARC_L2_START + SHARC_L2_MCAPI_SH2_OFFSET)

/* time to wait in polling loop before exitin */
#define WAIT_TIME_SEC 8

#ifdef SC589
/* location of bootrom that loops idle */
#define SHARC_LOOP_IDLE 0x5000a6
#endif

#ifdef SC573
/* location of bootrom that loops idle */
#define SHARC_LOOP_IDLE 0x500104
#endif

#define REG_ACCESS_SIZE (262144*1024)
#define REG_ADDRESS_START  (0x30000000)
#define REG_ADDRESS_SVECT1 (0x3108C030)
#define REG_ADDRESS_SVECT2 (0x3108C034)


extern struct appLdrSect{
  uint32_t tgt;
  uint8_t *sect;
  uint32_t sectsz;
} mySectsSharc1[], mySectsSharc2[];

enum memtype{
  sharcl1 = 0,
  sharedl2
};

/* Convert SHARC physical address to ARM virtual address */
volatile uint32_t * SharcPhys2ArmVirt(void *mappedmem_l1, void *mappedmem_l2, uint32_t sharc_s1_port, uint32_t sharc_l1_mps1_start, volatile uint32_t *sharc_addr) {
  // Check if it is in L1 or L2
  if ((uint32_t)sharc_addr >= SHARC_L2_START && (uint32_t)sharc_addr < SHARC_L2_END) {
    // Phys to Virt
    sharc_addr = (uint32_t *)((uint32_t)mappedmem_l2 + ((uint32_t)sharc_addr - SHARC_L2_START));
  }
  else {
    // Convert to MP address
    sharc_addr = (uint32_t *)(((uint32_t)sharc_addr & 0x003fffff) | sharc_s1_port);
    // Phys to Virt
    sharc_addr = (uint32_t *)((uint32_t)mappedmem_l1 + ((uint32_t)sharc_addr - sharc_l1_mps1_start));
  }

  return sharc_addr;
}

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
  void *mappedmem_l1, *mappedmem_l2;
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

  memfd = open("/dev/mem", O_RDWR | O_SYNC);
  if(memfd < 0){
    perror("error opening /dev/mem");
    exit(10);
  }
  mappedmem_l1 = mmap(0, SHARC_L1_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, sharc_l1_mps1_start);
  if(mappedmem_l1 == MAP_FAILED){
    perror("error mapping L1");
    exit(10);
  }
  mappedmem_l2 = mmap(0, SHARC_L2_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, SHARC_L2_START);
  if(mappedmem_l2 == MAP_FAILED){
    perror("error mapping L2");
    exit(10);
  }

  for(i=0; i<numsects; i++){
    tgt  = (uint8_t *)(appSects[i].tgt);
    data = appSects[i].sect;
    sectSz = appSects[i].sectsz;

    if(dbglvl > 2)
      printf("i: %d, tgt: %08x, size of data: %d\n", i, (int)tgt, sectSz);

    uTgt = (uint32_t)tgt;
    
    /* test if sect in L2 BW */
    if(uTgt >= SHARC_L2_START && uTgt < SHARC_L2_END){
      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem_l2 + ((unsigned int)tgt - SHARC_L2_START));
      
      /* copying to L2 */
      memcpy(tgt, data, sectSz);
    }
    /* test if sect in L2 NW */
    else if(uTgt >= SHARC_L2_NW_START && uTgt < SHARC_L2_NW_END){
      uTgt = uTgt << 2;                           // convert to BW address
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem_l2 + ((unsigned int)tgt - SHARC_L2_START));

      /* assert size of this section is mult of 4 */
      if((sectSz & 3) != 0)
        if(dbglvl > 2)
          printf("NW section sz not div by 4: %d\n", sectSz);

      for(j=0; j<sectSz; j+=4){
        uint8_t *d = &tgt[j], *s = &data[j];
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
      }
    }
    /* test if sect in L2 VISA */
    else if(uTgt >= SHARC_L2_VISA_START && uTgt < SHARC_L2_VISA_END){
      uTgt = (uTgt << 1) + SHARC_L2_VISA_ISA_OFFSET;    // convert to BW address
      tgt = (uint8_t *)uTgt;

      if(dbglvl > 2)
          printf("Translated target address: %04x\n", uTgt);

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem_l2 + ((unsigned int)tgt - SHARC_L2_START));

      /* assert size of this section is mult of 2 */
      if((sectSz & 1) != 0)
        if(dbglvl > 2)
          printf("VISA section sz not div by 2: %d\n", sectSz);

      for(j=0; j<sectSz; j+=2){
        uint8_t *d = &tgt[j], *s = &data[j];
        d[0] = s[1];
        d[1] = s[0];
      }
    }
    /* test if sect in L2 ISA */
    else if(uTgt >= SHARC_L2_ISA_START && uTgt < SHARC_L2_ISA_END){
      uTgt = (uTgt << 2) + SHARC_L2_VISA_ISA_OFFSET;    // convert to BW address
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem_l2 + ((unsigned int)tgt - SHARC_L2_START));

      /* assert size of this section is mult of 4 */
      if((sectSz & 3) != 0)
        if(dbglvl > 2)
          printf("ISA section sz not div by 4: %d\n", sectSz);

      for(j=0; j<sectSz; j+=4){
        uint8_t *d = &tgt[j], *s = &data[j];
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
      }
    }
#ifndef DISGARD_L1_SECTION
    /* test if sect in L1 BW */
    else if((uTgt >= SHARC_L1_B1_BW_START && uTgt < SHARC_L1_B1_BW_END) ||
        (uTgt >= SHARC_L1_B2_BW_START && uTgt < SHARC_L1_B2_BW_END) ||
        (uTgt >= SHARC_L1_B3_BW_START && uTgt < SHARC_L1_B3_BW_END) ||
        (uTgt >= SHARC_L1_B4_BW_START && uTgt < SHARC_L1_B4_BW_END)){
      uTgt = ((uTgt & 0x003fffff) | sharc_s1_port);
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem_l1 + ((unsigned int)tgt - sharc_l1_mps1_start));

      /* copying to L1 (BW) */
      memcpy(tgt, data, sectSz);
    }
    /* test if sect in L1 LW */
    else if((uTgt >= SHARC_L1_B1_LW_START && uTgt < SHARC_L1_B1_LW_END) ||
        (uTgt >= SHARC_L1_B2_LW_START && uTgt < SHARC_L1_B2_LW_END) ||
        (uTgt >= SHARC_L1_B3_LW_START && uTgt < SHARC_L1_B3_LW_END) ||
        (uTgt >= SHARC_L1_B4_LW_START && uTgt < SHARC_L1_B4_LW_END)){
      uTgt = uTgt << 3;                           // convert to BW address
      uTgt = ((uTgt & 0x003fffff) | sharc_s1_port);  // convert to MP address
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem_l1 + ((unsigned int)tgt - sharc_l1_mps1_start));


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
    /* test if sect in L1 NW/EP */
    else if((uTgt >= SHARC_L1_B1_NW_START && uTgt < SHARC_L1_B1_NW_END) ||
        (uTgt >= SHARC_L1_B2_NW_START && uTgt < SHARC_L1_B2_NW_END) ||
        (uTgt >= SHARC_L1_B3_NW_START && uTgt < SHARC_L1_B3_NW_END) ||
        (uTgt >= SHARC_L1_B4_NW_START && uTgt < SHARC_L1_B4_NW_END)){
      uTgt = uTgt << 2;                           // convert to BW address
      uTgt = ((uTgt & 0x003fffff) | sharc_s1_port);  // convert to MP address
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem_l1 + ((unsigned int)tgt - sharc_l1_mps1_start));

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
    /* test if sect in L1 SW */
    else if((uTgt >= SHARC_L1_B1_SW_START && uTgt < SHARC_L1_B1_SW_END) ||
        (uTgt >= SHARC_L1_B2_SW_START && uTgt < SHARC_L1_B2_SW_END) ||
        (uTgt >= SHARC_L1_B3_SW_START && uTgt < SHARC_L1_B3_SW_END) ||
        (uTgt >= SHARC_L1_B4_SW_START && uTgt < SHARC_L1_B4_SW_END)){
      uTgt = uTgt << 1;                           // convert to BW address
      uTgt = ((uTgt & 0x003fffff) | sharc_s1_port);  // convert to MP address
      tgt = (uint8_t *)uTgt;

      /* calc offset to added to mapped mem */
      tgt = (uint8_t *)((unsigned int)mappedmem_l1 + ((unsigned int)tgt - sharc_l1_mps1_start));

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
#endif
    else {
      printf("Error: unable to determine the destination of section %d!\n", i);
    }
  }

  /* shut down mmap */
  munmap(mappedmem_l2, SHARC_L2_SIZE);
  close(memfd);

  return;
}


#define USAGE_SC589                                                                 \
"usage:\n"                                                                          \
"  loadSharc_SC589 [options]\n"                                                     \
"options:\n"                                                                        \
"  -i [LDR file]       Sharc Binary LDR file to be \n"                              \
"  -s [Sharc Core #]   Sharc Core to load (1 or 2)\n"                               \
"  -d [debug level]    Amount of debug info 1, 2 or 3\n"                            \
"  -a [Arguments]      Arguments that will be passed to SHARC\n"                    \
"  -h                  Show this help message\n"                             

#define USAGE_SC573                                                                 \
"usage:\n"                                                                          \
"  loadSharc_SC573 [options]\n"                                                     \
"options:\n"                                                                        \
"  -i [LDR file]       Sharc Binary LDR file to be \n"                              \
"  -s [Sharc Core #]   Sharc Core to load (1 or 2)\n"                               \
"  -d [debug level]    Amount of debug info 1, 2 or 3\n"                            \
"  -a [Arguments]      Arguments that will be passed to SHARC\n"                    \
"  -h                  Show this help message\n"                             



/* OPTIONS DESCRIPTOR ================================ */
static struct option gLongOptions[] = {
  {"input", required_argument,  NULL,     'i'},
  {"sharc", required_argument,  NULL,     's'},
  {"debug", required_argument,  NULL,     'd'},
  {"args",  required_argument,  NULL,     'a'},
  {"help",  no_argument,        NULL,     'h'},
  {NULL,    0,                  NULL,      0}
};


void Usage() {
  #ifdef SC589
    fprintf(stdout, "%s", USAGE_SC589);
  #endif
   
  #ifdef SC573
    fprintf(stdout, "%s", USAGE_SC573);
  #endif
}

int main(int argc, char *argv[])
{
  int ccfd, ret, filesz;
  extern uint32_t sh1_progStartAddr, sh2_progStartAddr;
  uint32_t progStartAddr;
  uint32_t argStartAddr;
  FILE *fd;
  int memfd;
  int option_char;
  void *mappedmem_l1, *mappedmem_l2, *mappedmem_reg;
  extern volatile uint32_t *ptrMboxSh2Arm_sh1, *ptrMboxArm2Sh_sh1, *ptrMboxSh2Arm_sh2, *ptrMboxArm2Sh_sh2;
  volatile uint32_t *ptrMboxSh2Arm, *ptrMboxArm2Sh;
  extern uint32_t *ptrBootStream_sh1, *ptrBootStream_sh2;
  uint32_t *ptrBootStream;
  uint32_t *ptrArguments;
  extern volatile uint32_t *ptrLdSync2ArmByteCount_sh1, *ptrLdSync2ShByteCount_sh1, *ptrLdSync2ArmByteCount_sh2, *ptrLdSync2ShByteCount_sh2;
  volatile uint32_t *ptrLdSync2ArmByteCount, *ptrLdSync2ShByteCount;
  volatile uint32_t *ptrMboxSh2Arm_l2, *ptrMboxArm2Sh_l2, *ptrDataSh2Arm_l2, *ptrDataArm2Sh_l2;
  int reqSz, rdSz;
  int blockNum = 0;
  int flag = 0;
  time_t startt;
  int sharc_core=1;       // default to sharc core 1 
  const char empty_string[] = ""; 
  char * arguments = (char *)empty_string; // default to be empty 
  unsigned long sharc_s1_port, sharc_l1_mps1_start;
  int wait = 1;
  int c;

  if(argc < 2){
    printf("Nothing to be done...exiting\n");
    goto _exit; 
  } 

  while ((option_char = getopt_long(argc, argv, "i:s:d:a:h", gLongOptions, NULL)) != -1) {
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
    case 'a':
      arguments = optarg;
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
    ptrMboxSh2Arm_l2    = (uint32_t *)SHARC_L2_MCAPI_SH1_START;
    ptrMboxArm2Sh_l2    = ptrMboxSh2Arm_l2 + 1;
    ptrDataSh2Arm_l2    = ptrMboxSh2Arm_l2 + 2;
    ptrDataArm2Sh_l2    = ptrMboxSh2Arm_l2 + 3;
  }else if(sharc_core == 2){
    sharc_s1_port       = SHARC2_S1_PORT;
    sharc_l1_mps1_start = SHARC2_L1_MPS1_START;
    progStartAddr       = sh2_progStartAddr;
    ptrMboxSh2Arm       = ptrMboxSh2Arm_sh2;
    ptrMboxArm2Sh       = ptrMboxArm2Sh_sh2;
    ptrBootStream       = ptrBootStream_sh2;
    ptrLdSync2ArmByteCount = ptrLdSync2ArmByteCount_sh2;
    ptrLdSync2ShByteCount  = ptrLdSync2ShByteCount_sh2;
    ptrMboxSh2Arm_l2    = (uint32_t *)SHARC_L2_MCAPI_SH2_START;
    ptrMboxArm2Sh_l2    = ptrMboxSh2Arm_l2 + 1;
    ptrDataSh2Arm_l2    = ptrMboxSh2Arm_l2 + 2;
    ptrDataArm2Sh_l2    = ptrMboxSh2Arm_l2 + 3;
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

  /* open memory */
  memfd = open("/dev/mem", O_RDWR | O_SYNC);
  if(memfd < 0){
    perror("error opening /dev/mem for Register");
    goto _exit;
  }
  
#ifdef SC573
  
  mappedmem_reg = mmap(0, REG_ACCESS_SIZE , PROT_READ|PROT_WRITE, MAP_SHARED, memfd, REG_ADDRESS_START);
  if(mappedmem_reg == MAP_FAILED){
    perror("error mapping Register");
    goto _cleanup_closemap;
  }

  uint32_t *SVECT1 = (uint32_t *)(mappedmem_reg + (REG_ADDRESS_SVECT1  - REG_ADDRESS_START ));
  uint32_t *SVECT2 = (uint32_t *)(mappedmem_reg + (REG_ADDRESS_SVECT2  - REG_ADDRESS_START ));

  /* set svect[n] to spin on IDLE in boot rom */
  if(sharc_core ==1)
    *SVECT1 = SHARC_LOOP_IDLE;
  else
    *SVECT2 = SHARC_LOOP_IDLE;
  
#endif
        
#ifdef SC589   
  /* set svect[n] to spin on IDLE in boot rom */
  ret = ioctl(ccfd, CMD_SET_SVECT1, SHARC_LOOP_IDLE);
#endif
       
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
   
#ifdef SC573
  if(sharc_core ==1)
    *SVECT1 = progStartAddr;
  else
    *SVECT2 = progStartAddr;
#endif
        
#ifdef SC589
  /* set svect1 to beginning of app */
  ret = ioctl(ccfd, (sharc_core==1) ? CMD_SET_SVECT1 : CMD_SET_SVECT2, progStartAddr);
#endif
  
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
    perror("error opening /dev/mem for L1/L2");
    goto _exit;
  }

  mappedmem_l1 = mmap(0, SHARC_L1_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, sharc_l1_mps1_start);
  if(mappedmem_l1 == MAP_FAILED){
    perror("error mapping L1");
    goto _cleanup_closemap;
  }

  mappedmem_l2 = mmap(0, SHARC_L2_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, SHARC_L2_START);
  if(mappedmem_l2 == MAP_FAILED){
    perror("error mapping L2");
    goto _cleanup_closemap;
  }


  if(dbglvl > 2){
    printf("ptrMboxSh2Arm = %x\n", (uint32_t)ptrMboxSh2Arm);
    printf("ptrMboxArm2Sh = %x\n", (uint32_t)ptrMboxArm2Sh);
  }

  ptrMboxSh2Arm = SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrMboxSh2Arm);
  ptrMboxArm2Sh = SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrMboxArm2Sh);

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
    printf("ptrLSync2ArmByteCount = %x\n", (uint32_t)ptrLdSync2ArmByteCount);
    printf("ptrLdSync2ShByteCount = %x\n", (uint32_t)ptrLdSync2ShByteCount);
    printf("ptrBootStream         = %x\n", (uint32_t)ptrBootStream);
  }

  ptrLdSync2ArmByteCount = SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrLdSync2ArmByteCount);
  ptrLdSync2ShByteCount  = SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrLdSync2ShByteCount);
  ptrBootStream = (uint32_t *)SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrBootStream);

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

  /* At this stage, the sharc program should be up and running */
  /* If passing arguments is required, wait on the L2 mailbox */

  if(dbglvl > 2) {
    printf("ptrMboxSh2Arm_l2 = %x\n", ptrMboxSh2Arm_l2);
    printf("ptrMboxArm2Sh_l2 = %x\n", ptrMboxArm2Sh_l2);
    printf("ptrDataSh2Arm_l2 = %x\n", ptrDataSh2Arm_l2);
    printf("ptrDataArm2Sh_l2 = %x\n", ptrDataArm2Sh_l2);
  }

  ptrMboxSh2Arm_l2 = SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrMboxSh2Arm_l2);
  ptrMboxArm2Sh_l2 = SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrMboxArm2Sh_l2);
  ptrDataSh2Arm_l2 = SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrDataSh2Arm_l2);
  ptrDataArm2Sh_l2 = SharcPhys2ArmVirt(mappedmem_l1, mappedmem_l2, sharc_s1_port, sharc_l1_mps1_start, ptrDataArm2Sh_l2);

  if(dbglvl > 0)
    printf("Waiting application to start up.\n");
  time(&startt);
  while (*ptrMboxSh2Arm_l2 != 0x5555) {
    if(difftime(time(NULL), startt) > WAIT_TIME_SEC){       // exit if we wait too long
      if(dbglvl > 0)
        printf("time expired waiting for Sharc msg...exiting\n");
      goto _cleanup;
    }
    printf("Values: %04x, %04x\n", *ptrMboxSh2Arm_l2, *ptrMboxArm2Sh_l2);
    usleep(500000);
  }
  if(dbglvl > 0)
    printf("Application has started up.\n");
  
  
  if(dbglvl > 0)
    printf("Arguments to be passed: %s\n", arguments);
  
  /* send the length */
  uint32_t length;
  length = (uint32_t)strlen(arguments);
  if(dbglvl > 0)
    printf("String length = %d\n", length);

  *ptrDataArm2Sh_l2 = length;
  *ptrMboxArm2Sh_l2 = 0x5555;

  /* wait the memory allocation result */
  while (*ptrMboxSh2Arm_l2 != 0xbbbb) {
    /* could because it failed */
    if (*ptrMboxSh2Arm_l2 == 0xdead) {
      *ptrMboxArm2Sh_l2 = 0xbeef;
      goto _cleanup;
    }
  }

  /* The address can be in L1, also can be in L2 */
  argStartAddr = *ptrDataSh2Arm_l2;
  if (dbglvl > 2)
    printf("argStartAddr: 0x%04x\n", argStartAddr);
  if ((argStartAddr >= SHARC_L1_START) && (argStartAddr < (SHARC_L1_START + SHARC_L1_SIZE))) {
    if (dbglvl > 2)
      printf("Allocated in L1.\n", argStartAddr);
    argStartAddr = ((argStartAddr & 0x003fffff) | sharc_s1_port);
    ptrArguments = (uint32_t *)((uint32_t)mappedmem_l1 + (argStartAddr - sharc_l1_mps1_start));
  }
  else if ((argStartAddr >= SHARC_L1_START) && (argStartAddr < (SHARC_L1_START + SHARC_L1_SIZE))) {
    if (dbglvl > 2)
      printf("Allocated in L2.\n", argStartAddr);
    ptrArguments = (uint32_t *)((uint32_t)mappedmem_l2 + ((uint32_t)argStartAddr - SHARC_L2_START));
  }
  else {
    if(dbglvl > 0)
      printf("unable to determine the region where arguments is allocated...exiting\n");
    goto _cleanup;
  }

  /* ready to copy the arguments */
  memcpy(ptrArguments, arguments, strlen(arguments));
  *ptrMboxArm2Sh_l2 = 0x6666;

  /* end here, the mailbox is left for the SHARC to clear */


_cleanup:
_cleanup_unmap: // Normal
  munmap(mappedmem_l2, SHARC_L2_SIZE);
_cleanup_unmap_l1: // L1 map success, L2 map fail, jumps here
  munmap(mappedmem_l1, SHARC_L1_SIZE);
#ifdef SC573
  munmap(mappedmem_reg, REG_ACCESS_SIZE);
#endif
_cleanup_closemap: // L1 map fail, jumps here
  close(memfd);

_exit:
  return 0;

}
