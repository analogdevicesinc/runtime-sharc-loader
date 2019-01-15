/*****************************************************************************
 * Griffin_ADC_DAC_Codes_Core1.c
 *****************************************************************************/

#include <sys/platform.h>
#include "Griffin_ADC_DAC_Codes_Core1.h"
#include "ADAU_1962Common.h"
#include "ADAU_1979Common.h"
#include <stdio.h>
#include<math.h>
#include <cdefSC589.h>
#include "Sport_Validation_API_Core.h"
#include "PORTsTWIConfig.h"
#include "sruSC589.h"
#include "recv_arg.h"
int temp1;
int temp2;
int temp3;
int temp4;
int temp5;
int temp6;

struct Config_Table
{
	short Reg_Add;
	char  Value;
};

char Config_read_DAC[28];
char Config_read_ADC[16];
int count;
void DMA_HANDLER(int iid, void *handlerArg)
{
	int i;
	/* Ping pong buffering */
	*pREG_DMA11_STAT|=BITM_DMA_STAT_IRQDONE;
	count++;

	if(count==1)
	{
	   for(i=0;i<COUNT;i++)
	   {

		   int_SP0ABuffer1[i]=int_SP0ABuffer4[i];/*Copy ADC buffer to DAC buffer */

	   }
	}

	if(count==2)
		{
		   for(i=0;i<COUNT;i++)
		   {

			   int_SP0ABuffer2[i]=int_SP0ABuffer5[i];/* Copy ADC buffer to DAC buffers */

		   }
		   count=0;
		}



}
struct Config_Table Config_array_DAC[28] = {
		      //{     ADAU1962_PLL_CTL_CTRL0,   0x05},
		      //{     ADAU1962_PLL_CTL_CTRL1,   0x20},
		   	    {     ADAU1962_PDN_CTRL_1,		0x00},
		   	    {     ADAU1962_PDN_CTRL_2,	    0xff},
		   	    {     ADAU1962_PDN_CTRL_3,	    0x0f },
		   	    {     ADAU1962_DAC_CTRL0,		0x01},
		   	    {     ADAU1962_DAC_CTRL1,		0x01 },
		   	    {     ADAU1962_DAC_CTRL2,		0x00 },
		   	    {     ADAU1962_DAC_MUTE1,	    0x0},
		   	    {     ADAU1962_DAC_MUTE2,	    0x00},
		   	    {     ADAU1962_MSTR_VOL,		0x00 },
			    {     ADAU1962_DAC1_VOL,	    0x00 } ,
			    {     ADAU1962_DAC2_VOL,		0x00  },
				{     ADAU1962_DAC3_VOL,		0x00 },
				{     ADAU1962_DAC4_VOL,		0x00 },
				{     ADAU1962_DAC5_VOL,		0x00 },
				{     ADAU1962_DAC6_VOL,		0x00  },
				{     ADAU1962_DAC7_VOL,		0x00 },
				{     ADAU1962_DAC8_VOL,	    0x00 },
				{     ADAU1962_DAC9_VOL,		0x00  },
				{     ADAU1962_DAC10_VOL,		0x00},
				{     ADAU1962_DAC11_VOL,		0x00},
				{     ADAU1962_DAC12_VOL,		0x00 },
				{     ADAU1962_PAD_STRGTH,		0x00 },
				{     ADAU1962_DAC_PWR1,		0xaa },
				{     ADAU1962_DAC_PWR2,		0xaa },
				{     ADAU1962_DAC_PWR3,		0xaa },
				{     ADAU1962_PDN_CTRL_2,	    0x00},  //Power up DAC
				{     ADAU1962_PDN_CTRL_3,	    0x00 }, //Power up DAC
				{     ADAU1962_DAC_CTRL0,		0x00}

};

struct Config_Table Config_array_ADC[16] = {

		// {ADAU1979_REG_POWER			,		0x00},
		// {ADAU1979_REG_PLL				,	0x01},
		 {ADAU1979_REG_BOOST			,	0x00},
		 {ADAU1979_REG_MICBIAS			,	0x00},
		 {ADAU1979_REG_BLOCK_POWER_SAI	,	0x30},
		 {ADAU1979_REG_SAI_CTRL0		,		0x02},
		 {ADAU1979_REG_SAI_CTRL1		,		0x00},
		 {ADAU1979_REG_CMAP12			,		0x01},
		 {ADAU1979_REG_CMAP34			,		0x00},
		 {ADAU1979_REG_SAI_OVERTEMP		,	0x30},
		 {ADAU1979_REG_POST_ADC_GAIN1 	,	0xA0},
		 {ADAU1979_REG_POST_ADC_GAIN2 	,	0xA0},
		 {ADAU1979_REG_POST_ADC_GAIN3 	,	0xA0},
		 {ADAU1979_REG_POST_ADC_GAIN4 	,	0xA0},
		 {ADAU1979_REG_ADC_CLIP			,	0x00},//no clip
		 {ADAU1979_REG_DC_HPF_CAL		,	0x00},// filter off
		 {ADAU1979_REG_BLOCK_POWER_SAI	,	0x33},
		 {ADAU1979_REG_MISC_CONTROL		,	0x00}
};
void ADAU_1962_Pllinit();
void ADAU_1979_Pllinit();
void Switch_Configurator();
void ADAU_1962_init();
void ADAU_1979_init();
void DAI_init();

int count=0;

int main(int argc, char *argv[])
{
	/* Receive arguments from the RSL */
	recv_arg(&argc, &argv);

	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */
	adi_initComponents();
    /* PADS0 DAI0 Port Input Enable Control Register */
    *pREG_PADS0_DAI0_IE = (unsigned int) 0x001FFFFE;

    /* PADS0 DAI1 Port Input Enable Control Register */
    *pREG_PADS0_DAI1_IE = (unsigned int) 0x001FFFFE;

    /* Enable SPU for DMAs */
	*pREG_SPU0_SECUREP74=0x3;
     *pREG_SPU0_SECUREP75=0x3;

  /* Interrupt Handler for Sport interfaced to ADC */
	adi_int_InstallHandler(INTR_SPORT4_B_DMA,DMA_HANDLER,0,true);

	/* Enable Softconfig switches */
	Switch_Configurator();
	/* DAI configuration */
	DAI_init();
	/* DAC init */
	ADAU_1962_init();
	/* ADC init */
	ADAU_1979_init();
	/* Sport intialization */
	Sport_init();
	while(1);
}

void Write_TWI_8bit_Reg(unsigned char Reg_ID, unsigned char Tx_Data)
{
        *pREG_TWI0_TXDATA8 = (Reg_ID & 0xFF);
        *pREG_TWI0_TXDATA8 = (Tx_Data & 0xFF);
        *pREG_TWI0_MSTRCTL = ((2 << 6) | ENUM_TWI_MSTRCTL_TX | BITM_TWI_MSTRCTL_EN);
        while(!(*pREG_TWI0_ISTAT &      BITM_TWI_ISTAT_TXSERV));
        *pREG_TWI0_ISTAT  |= BITM_TWI_ISTAT_TXSERV;
    while(!(*pREG_TWI0_ISTAT & BITM_TWI_ISTAT_MCOMP));
        *pREG_TWI0_ISTAT  |= BITM_TWI_ISTAT_TXSERV | BITM_TWI_ISTAT_MCOMP;
}

unsigned char Read_TWI_8bit_Reg(unsigned char Reg_ID)
{
        unsigned char Rx_Data;
        *pREG_TWI0_TXDATA8 = (Reg_ID & 0xFF);
        *pREG_TWI0_MSTRCTL = ((1 << 6) | BITM_TWI_MSTRCTL_RSTART | ENUM_TWI_MSTRCTL_TX | BITM_TWI_MSTRCTL_EN);
        while(!(*pREG_TWI0_ISTAT & BITM_TWI_ISTAT_MCOMP));
        *pREG_TWI0_ISTAT  |= (BITM_TWI_ISTAT_TXSERV | BITM_TWI_ISTAT_MCOMP);

        *pREG_TWI0_MSTRCTL = ((1 << 6) | ENUM_TWI_MSTRCTL_RX | BITM_TWI_MSTRCTL_EN);
        while(!(*pREG_TWI0_ISTAT &      BITM_TWI_ISTAT_RXSERV));
        Rx_Data = (*pREG_TWI0_RXDATA8 & 0xFF);
        *pREG_TWI0_ISTAT |=     BITM_TWI_ISTAT_RXSERV;
    while(!(*pREG_TWI0_ISTAT & BITM_TWI_ISTAT_MCOMP));
        *pREG_TWI0_ISTAT |= BITM_TWI_ISTAT_RXSERV | BITM_TWI_ISTAT_MCOMP;

        return Rx_Data;
}





void Init_TWI0(void)
{

	  *pREG_TWI0_CTL=0x008A;
			       *pREG_TWI0_CLKDIV=0x3131;
			       *pREG_TWI0_MSTRADDR=0x38;
			       *pREG_TWI0_ISTAT =0xffff;
			       *pREG_TWI0_MSTRSTAT =0xffff;

		           *pREG_TWI0_FIFOCTL=0;
			       *pREG_TWI0_IMSK=0x00u;
}
/*****************************************************************************************************************************/

void ADAU_1979_Pllinit()
{

	int status,delay1=0xffff;;
	 *pREG_TWI0_MSTRADDR     =       0x11;
	 Write_TWI_8bit_Reg(ADAU1979_REG_POWER,0x01);
	Write_TWI_8bit_Reg(ADAU1979_REG_PLL,0x03);
	status=Read_TWI_8bit_Reg(ADAU1979_REG_PLL);
	 printf("\n ADC Configured Register %x Configured value %x, Return value %x ",ADAU1979_REG_PLL,0x3,status );
	while(delay1--)
			{
				asm("nop;");
			}
	while(!((status & 0x80)>>7)  )
	{
		status=Read_TWI_8bit_Reg(ADAU1979_REG_PLL);
		asm("nop;");
	}

}
void ADAU_1962_init()
{	int i;
    Init_TWI0();
     ADAU_1962_Pllinit();

		for(i=0;i<28;i++)
		{
			Write_TWI_8bit_Reg(Config_array_DAC[i].Reg_Add,Config_array_DAC[i].Value);
			Config_read_DAC[i]=Read_TWI_8bit_Reg(Config_array_DAC[i].Reg_Add);
		    printf("\n DAC Configured Register %x Configured value %x, Return value %x ",Config_array_DAC[i].Reg_Add,Config_array_DAC[i].Value, Config_read_DAC[i] );
             if(Config_array_DAC[i].Value!= Config_read_DAC[i])
				    {
				      printf("\n fail");
				    }



		}


}

void ADAU_1962_Pllinit()
{
	int status,delay1=0xffff;
	 *pREG_TWI0_MSTRADDR     =       0x04;
		Write_TWI_8bit_Reg(ADAU1962_PLL_CTL_CTRL0,0x01);
		while(delay1--)
				{
					asm("nop;");
				}

		Write_TWI_8bit_Reg(ADAU1962_PLL_CTL_CTRL0,0x05);

		delay1=0xffff;
				while(delay1--)
						{
							asm("nop;");
						}

		Write_TWI_8bit_Reg(ADAU1962_PLL_CTL_CTRL1,0x22);
		delay1=0xffff;
		while(delay1--)
		{
			asm("nop;");
		}
		status=Read_TWI_8bit_Reg(ADAU1962_PLL_CTL_CTRL1);
		while(!((status & 0x4)>>2)  )
		{
			status=Read_TWI_8bit_Reg(ADAU1962_PLL_CTL_CTRL1);
		}
}
void ADAU_1979_init()
{	int i;
     Init_TWI0();
     ADAU_1979_Pllinit();
		for(i=0;i<16;i++)
		{
			Write_TWI_8bit_Reg(Config_array_ADC[i].Reg_Add,Config_array_ADC[i].Value);
			Config_read_ADC[i]=Read_TWI_8bit_Reg(Config_array_ADC[i].Reg_Add);
		    printf("\n ADC Configured Register %x Configured value %x, Return value %x ",Config_array_ADC[i].Reg_Add,Config_array_ADC[i].Value, Config_read_ADC[i] );
             if(Config_array_ADC[i].Value!= Config_read_ADC[i])
				    {
				      printf("\n fail");
				    }



		}
}

void buffer_init()
{
	int i;
		for(i=0;i<COUNT;i++)
		{
			int_SP0ABuffer1[i]=int_SP0ABuffer2[i]=(2*8388608)*sin(0.01*i);
		}
}
void Sport_init()
{

	   buffer_init();
		int DMA10_Config=(		((0		<< BITP_DMA_CFG_WNR  ) & BITM_DMA_CFG_WNR  ) |
		   						((0 	<< BITP_DMA_CFG_TWOD ) & BITM_DMA_CFG_TWOD ) |
		   						((0 	<< BITP_DMA_CFG_INT  ) & BITM_DMA_CFG_INT  ) |
		   						((MSIZE 	<< BITP_DMA_CFG_MSIZE) & BITM_DMA_CFG_MSIZE) |
		   						((PSIZE 	<< BITP_DMA_CFG_PSIZE) & BITM_DMA_CFG_PSIZE) |
		   						((0	 	<< BITP_DMA_CFG_SYNC ) & BITM_DMA_CFG_SYNC ) |
		   						((0	<< BITP_DMA_CFG_TWAIT) & BITM_DMA_CFG_TWAIT) |
		   						((0	<< BITP_DMA_CFG_TRIG ) & BITM_DMA_CFG_TRIG ) |
		   						ENUM_DMA_CFG_FETCH02);


		int DMA11_Config=(		((1		<< BITP_DMA_CFG_WNR  ) & BITM_DMA_CFG_WNR  ) |
			   						((0 	<< BITP_DMA_CFG_TWOD ) & BITM_DMA_CFG_TWOD ) |
			   						((1 	<< BITP_DMA_CFG_INT  ) & BITM_DMA_CFG_INT  ) |
			   						((MSIZE 	<< BITP_DMA_CFG_MSIZE) & BITM_DMA_CFG_MSIZE) |
			   						((PSIZE 	<< BITP_DMA_CFG_PSIZE) & BITM_DMA_CFG_PSIZE) |
			   						((0	 	<< BITP_DMA_CFG_SYNC ) & BITM_DMA_CFG_SYNC ) |
			   						((0	<< BITP_DMA_CFG_TWAIT) & BITM_DMA_CFG_TWAIT) |
			   						((1	<< BITP_DMA_CFG_TRIG ) & BITM_DMA_CFG_TRIG ) |
			   						ENUM_DMA_CFG_FETCH02);


temp1 =(uint32_t)&iDESC_LIST_1_SP1A|MP_OFFSET ;
temp2 =(uint32_t)&iSRC_LIST_1_SP0A |MP_OFFSET ;



temp3 =(uint32_t)&iDESC_LIST_2_SP1A|MP_OFFSET ;
temp4 =(uint32_t)&iSRC_LIST_2_SP0A |MP_OFFSET ;



*pREG_DMA10_DSCPTR_NXT = (uint32_t)&iDESC_LIST_1_SP1A|MP_OFFSET;
*pREG_DMA11_DSCPTR_NXT = (uint32_t)&iSRC_LIST_1_SP0A |MP_OFFSET;


iDESC_LIST_1_SP1A.Next_Desc=(void *)temp3;
iDESC_LIST_1_SP1A.Start_ADDR=(uint32_t)int_SP0ABuffer1|MP_OFFSET;

iDESC_LIST_2_SP1A.Next_Desc=(void *)temp1;
iDESC_LIST_2_SP1A.Start_ADDR=(uint32_t)int_SP0ABuffer2|MP_OFFSET;


iSRC_LIST_1_SP0A.Next_Desc=(void *) temp4;
iSRC_LIST_1_SP0A.Start_ADDR=(uint32_t)int_SP0ABuffer4|MP_OFFSET;

iSRC_LIST_2_SP0A.Next_Desc=(void *) temp2;
iSRC_LIST_2_SP0A.Start_ADDR=(uint32_t)int_SP0ABuffer5|MP_OFFSET;




	//(&iDESC_LIST_1_SP1A)|MP_OFFSET;
	*pREG_DMA10_XCNT=COUNT;
		*pREG_DMA11_XCNT=COUNT;

		*pREG_DMA10_XMOD=4;
		*pREG_DMA11_XMOD=4;


		*pREG_DMA10_CFG = DMA10_Config | (4<< BITP_DMA_CFG_FLOW) ;
		*pREG_DMA11_CFG = DMA11_Config | (4<< BITP_DMA_CFG_FLOW) ;


		    Sport_mode_config(4,A,1);


			// Clock configuration of 4 sport
			Sport_clockConfig(4,A,0x20,0,1,0);


		   // Frame sync configuration
		   	Sport_frameConfig(4,A,SLEN,0,lateFS,lowFS,dataIndFS,edgedetFS,FSR);


		  // Data Configuration

		     Sport_dataConfig(4,A,23,LSBfirst,dataType,0,1,packEnable,0);

		    //Enable Sport and Associated DMA

		   //  Sport_DMA_enable(0,1);




		   Sport_mode_config(4,B,0);


		    		// Clock configuration of 4 sport
		   Sport_clockConfig(4,B,0x20,0,1,0);


		    	   // Frame sync configuration
		   Sport_frameConfig(4,B,31,0,lateFS,lowFS,dataIndFS,edgedetFS,FSR);


		    	  // Data Configuration

		   Sport_dataConfig(4,B,23,LSBfirst,1,0,1,packEnable,0);

		    	    //Enable Sport and Associated DMA
		  Sport_DMA_enable(10,1);
		   Sport_enable(4,A,0,1);

		   Sport_DMA_enable(11,1);
		   Sport_enable(4,B,0,1);


}

void Switch_Configurator()
{
	int delay11=0xffff;

	*pREG_PORTA_FER_CLR  = BITM_PORT_FER_CLR_PX14 ;
	*pREG_PORTA_DATA_CLR = BITM_PORT_DATA_SET_PX14;
	*pREG_PORTA_DIR_SET  = BITM_PORT_DIR_SET_PX14;
	Switch_Enable(ADAU1962_EN,0);
    Switch_Enable(ADAU1979_EN,0);
    Switch_Enable(AUDIO_JACK_SEL,1);
    while(delay11--)
    {
       asm("nop;");
    }

    *pREG_PORTA_DATA_SET = BITM_PORT_DATA_SET_PX14;


    /* wait for Codec to up */
    delay11=0xffff;
    while(delay11--)
       {
          asm("nop;");
       }



}

void DAI_init()
{

		    SRU2(LOW,DAI1_PBEN02_I);

	        SRU2(DAI1_PB02_O,SPT4_ACLK_I);
		    SRU2(DAI1_PB02_O,SPT4_BCLK_I);

		    SRU2(DAI1_PB04_O,SPT4_AFS_I);
		    SRU2(DAI1_PB04_O,SPT4_BFS_I);
		    SRU2(LOW,DAI1_PBEN04_I);



		    SRU2(SPT4_AD0_O,DAI1_PB01_I);

		    SRU2(HIGH,DAI1_PBEN01_I);



		    SRU2(DAI1_PB02_O,DAI1_PB12_I);

		    SRU2(HIGH,DAI1_PBEN12_I);

		    SRU2(DAI1_PB04_O,DAI1_PB20_I);

		    SRU2(HIGH,DAI1_PBEN20_I);



		    SRU2(DAI1_PB06_O,SPT4_BD0_I);
		    SRU2(LOW,DAI1_PBEN06_I);


}
