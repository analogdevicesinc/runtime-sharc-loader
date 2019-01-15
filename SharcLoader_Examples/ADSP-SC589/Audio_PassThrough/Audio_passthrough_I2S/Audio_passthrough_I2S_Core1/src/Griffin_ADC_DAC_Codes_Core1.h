/*****************************************************************************
 * Griffin_ADC_DAC_Codes_Core1.h
 *****************************************************************************/

#ifndef __GRIFFIN_ADC_DAC_CODES_CORE1_H__
#define __GRIFFIN_ADC_DAC_CODES_CORE1_H__

/* Add your custom header content here */
#include <stdint.h>
#define SLEN 23
#define COUNT 200

int ISRcounter;
int DMA_Finish;
int FLOW;

int MSIZE=2;
int PSIZE=2;
typedef struct
  {
 	 void *Next_Desc;
 	  int *Start_ADDR;
  } SPORT_DMA_DESC_int;




#define L1

int iEdge=1;
int gClock;

int lateFS;
int lowFS=0;
int dataIndFS;
int edgedetFS;
int FSR;


#define MP_OFFSET	(uint32_t)0x28000000
#define MP_OFFSET1	0x28000000

section("seg_dmda_bw")
int   int_SP0ABuffer1[COUNT];


section("seg_dmda_bw")
int   int_SP0ABuffer2[COUNT];


section("seg_dmda_bw")
int   int_SP0ABuffer3[COUNT];


section("seg_dmda_bw")
int   int_SP0ABuffer4[COUNT];


section("seg_dmda_bw")
int   int_SP0ABuffer5[COUNT];


section("seg_dmda_bw")
int   int_SP0ABuffer6[COUNT];


  section("seg_dmda_bw")
 SPORT_DMA_DESC_int iDESC_LIST_1_SP1A;


  section("seg_dmda_bw")
  SPORT_DMA_DESC_int iDESC_LIST_2_SP1A;


  section("seg_dmda_bw")
  SPORT_DMA_DESC_int iSRC_LIST_1_SP0A;


  section("seg_dmda_bw")
  SPORT_DMA_DESC_int iSRC_LIST_2_SP0A;

#endif

int LSBfirst;
int dataType=1;
int packEnable;


 SPORT_DMA_DESC_int iDESC_LIST_1_SP1A = {(&iDESC_LIST_2_SP1A ), (int_SP0ABuffer1) };

SPORT_DMA_DESC_int iDESC_LIST_2_SP1A = {(&iDESC_LIST_1_SP1A), (int_SP0ABuffer2)};




  SPORT_DMA_DESC_int iSRC_LIST_1_SP0A = {(&iSRC_LIST_2_SP0A), (int_SP0ABuffer4)};
  SPORT_DMA_DESC_int iSRC_LIST_2_SP0A = {(&iSRC_LIST_1_SP0A), (int_SP0ABuffer5)};



