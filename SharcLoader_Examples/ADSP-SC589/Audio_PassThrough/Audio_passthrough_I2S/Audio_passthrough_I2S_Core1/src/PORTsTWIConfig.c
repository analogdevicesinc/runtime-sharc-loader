/***************************************************************************************
* 	File name : PortsTWIConfig.C
* 	Purpose: Includes the macros, variables and function definitions needed for configuring
* 			 the TWI switch
* 	Author: Vinod B
* 	VDSP version used for testing: Andromeda Beta(Oct 14th 2011)
* 	Hardware used: BF609 EZ-KIT
* 	Connection details :  N/A
* 	Guidelines for change: N/A
*****************************************************************************************/
#include "PortsTWIConfig.h"

// Local function declarations


char 	DATA_I2C_0_IODIR_A= 0xff ;
char 	DATA_I2C_0_OLAT_A =	0xff ;
char 	DATA_I2C_0_IODIR_B= 0xff ;
char 	DATA_I2C_0_OLAT_B =	0xff ;
char 	DATA_I2C_1_IODIR_A= 0xff ;
char 	DATA_I2C_1_OLAT_A =	0xff ;
char 	DATA_I2C_1_IODIR_B= 0xff ;
char 	DATA_I2C_1_OLAT_B =	0xff ;
char 	DATA_I2C_2_IODIR_A= 0xff ;
char 	DATA_I2C_2_OLAT_A =	0xff ;
char 	DATA_I2C_2_IODIR_B= 0xff ;
char 	DATA_I2C_2_OLAT_B =	0xff ;


/*****************************************************************************************
*	Function name:  Switch_Enable
*	Description:	This Function enables the corresponding switch on the TWI switch.
*		Parameter	 		|	Description		 					| Valid values      |
*	    signal				|  										| 0 - 33			|
*		state				|  										| 0 or 1			|
*   Return value:    None.
*******************************************************************************************/
void Switch_Enable(int signal, int state)
{
	TWI0_Init();
        
#ifdef __ADSPBF609__
	switch(signal)
	{
		case POS_CAN_EN:
				TWI_Configure(0, 'A', 0, state);
				break;
		case POS_CAN_STB:
				TWI_Configure(0, 'A', 1, state);
				break;
		case POS_CAN0_ERR_EN:
				TWI_Configure(0, 'A', 2, state);
				break;
		case POS_CAN0RX_EN:
				TWI_Configure(0, 'A', 3, state);
				break;
		case POS_CNT0UD_EN:
				TWI_Configure(0, 'A', 4, state);
				break;
		case POS_CNT0DG_EN:
				TWI_Configure(0, 'A', 5, state);
				break;
		case POS_CNT0ZM_EN:
				TWI_Configure(0, 'A', 6, state);
				break;
		case POS_RMII_CLK_EN:
				TWI_Configure(0, 'A', 7, state);
				break;
		case POS_UART0_RTS_EN:
				TWI_Configure(0, 'B', 0, state);
				break;
		case POS_UART0RX_EN:
				TWI_Configure(0, 'B', 1, state);
				break;
		case POS_UART0CTS_EN:
				TWI_Configure(0, 'B', 2, state);
				break;
		case POS_UART0CTS_RTS_LPBK:
				TWI_Configure(0, 'B', 3, state);
				break;
		case POS_UART0CTS_RST_EN:
				TWI_Configure(0, 'B', 4, state);
				break;
		case POS_UART0CTS_146_EN:
				TWI_Configure(0, 'B', 5, state);
				break;
		case POS_TEMP_IRQ_EN:
				TWI_Configure(0, 'B', 6, state);
				break;
		case POS_TEMP_THERM_EN:
				TWI_Configure(0, 'B', 7, state);
				break;
		case POS_OVERRIDE_EBIU_LP0_BOOT:
				TWI_Configure(1, 'A', 0, state);
				break;
		case POS_EBIU_EPPI2_LP1_SWITCH:
				TWI_Configure(1, 'A', 1, state);
				break;
		case POS_EBIU_LP0_EN:
				TWI_Configure(1, 'A', 3, state);
				break;
		case POS_LED1_GPIO1_EN:
				TWI_Configure(1, 'A', 4, state);
				break;
		case POS_LED1_GPIO2_EN:
				TWI_Configure(1, 'A', 5, state);
				break;
		case POS_LED1_GPIO3_EN:
				TWI_Configure(1, 'A', 6, state);
				break;
		case POS_LED1_GPIO4_EN:
				TWI_Configure(1, 'A', 7, state);
				break;
		case POS_PUSHBUTTON1_EN:
				TWI_Configure(1, 'B', 0, state);
				break;
		case POS_PUSHBUTTON2_EN:
				TWI_Configure(1, 'B', 1, state);
				break;
		case POS_SD_CD_EN:
				TWI_Configure(1, 'B', 2, state);
				break;
		case POS_SD_WP_EN:
				TWI_Configure(1, 'B', 3, state);
				break;
		case POS_SPIFLASH_CS_EN:
				TWI_Configure(1, 'B', 4, state);
				break;
		case POS_SPI0D2_EN:
				TWI_Configure(1, 'B', 5, state);
				break;
		case POS_SPI0D3_EN:
				TWI_Configure(1, 'B', 6, state);
				break;
		case POS_PHYINT_EN:
				TWI_Configure(2, 'A', 0, state);
				break;
		case POS_PHY_PWR_DWN_INT:
				TWI_Configure(2, 'A', 1, state);
				break;
		case POS_PHYAD0:
				TWI_Configure(2, 'A', 2, state);
				break;
		case POS_ETHERNET_DISABLE:
				TWI_Configure(2, 'A', 3, state);
				break;
	}
#endif
        
#ifdef __ADSPCM40X__
	switch(signal)
	{
		case POS_SPIFLASH_CS_EN:
				TWI_Configure(0, 'A', 0, state);
				break;
		case POS_SPI0D2_EN:
				TWI_Configure(0, 'A', 1, state);
				break;
		case POS_SPI0D3_EN:
				TWI_Configure(0, 'A', 2, state);
				break;
		case POS_UART0RTS_EN:
				TWI_Configure(0, 'A', 3, state);
				break;
		case POS_UART0RX_EN:
				TWI_Configure(0, 'A', 4, state);
				break;
		case POS_UART0CTS_EN:
				TWI_Configure(0, 'A', 5, state);
				break;
		case POS_UART0CTS_RTS_LPBK:
				TWI_Configure(0, 'A', 6, state);
				break;
		case POS_UART0CTS_RTS_EN:
				TWI_Configure(0, 'A', 7, state);
				break;
		case POS_CAN0_EN:
				TWI_Configure(0, 'B', 0, state);
				break;
		case POS_PUSHBUTTON1_EN:
				TWI_Configure(0, 'B', 1, state);
				break;
		case POS_PUSHBUTTON2_EN:
				TWI_Configure(0, 'B', 2, state);
				break;
		case POS_LED1_GPIO1_EN:
				TWI_Configure(0, 'B', 3, state);
				break;
		case POS_LED2_GPIO2_EN:
				TWI_Configure(0, 'B', 4, state);
				break;
		case POS_LED3_GPIO3_EN:
				TWI_Configure(0, 'B', 5, state);
				break;
		case POS_LED4_GPIO4_EN:
				TWI_Configure(0, 'B', 6, state);
				break;
		case POS_RS485_RE:
				TWI_Configure(2, 'A', 0, state);
				break;
		case POS_RS485_DE:
				TWI_Configure(2, 'A', 1, state);
				break;
		case POS_CAN1_EN:
				TWI_Configure(2, 'A', 2, state);
				break;
		case POS_ETH0_MDIO_BUS_EN:
				TWI_Configure(2, 'A', 3, state);
				break;
		case POS_ETHERNET_EN:
				TWI_Configure(2, 'A', 4, state);
				break;
		case POS_PHYINT_EN:
				TWI_Configure(2, 'A', 5, state);
				break;
		case POS_RMII_CLK_EN:
				TWI_Configure(2, 'A', 6, state);
				break;
		case POS_WAKE_PUSHBUTTON_EN:
				TWI_Configure(2, 'A', 7, state);
				break;
		case DATA_LOW_EXP_EN:
				TWI_Configure(2, 'B', 0, state);
				break;
		case DATA_HIGH_EXP_EN:
				TWI_Configure(2, 'B', 1, state);
				break;
		case TRACE_EXP_EN:
				TWI_Configure(2, 'B', 2, state);
				break;
		case PB_08_09_EXP_EN:
				TWI_Configure(2, 'B', 3, state);
				break;                                
		case ETH_EXP_EN:
				TWI_Configure(2, 'B', 4, state);
				break; 
        }
#endif

#ifdef __ADSPBF707__
	switch(signal)
	{
		case SPIFLASH_CS_EN:
				TWI_Configure(1, 'A', 0, state);
				break;
		case SPIFLASH_D2_EN:
				TWI_Configure(1, 'A', 1, state);
				break;
		case SPIFLASH_D3_EN:
				TWI_Configure(1, 'A', 2, state);
				break;
		case SD_CD_EN:
				TWI_Configure(1, 'A', 4, state);
				break;
		case SD_WP_EN:
				TWI_Configure(1, 'A', 5, state);
				break;
		case CAN0_EN:
				TWI_Configure(1, 'B', 2, state);
				break;
		case CAN0_STB:
				TWI_Configure(1, 'B', 3, state);
				break;
		case CAN1_EN:
				TWI_Configure(1, 'B', 4, state);
				break;
		case CAN1_STB:
				TWI_Configure(1, 'B', 5, state);
				break;
		case CAN0_ERR_EN:
				TWI_Configure(1, 'B', 6, state);
				break;
		case CAN1_ERR_EN:
				TWI_Configure(1, 'B', 7, state);
				break;
		case CAN0_TX_EN:
				TWI_Configure(2, 'A', 0, state);
				break;
		case CAN1_TX_EN:
				TWI_Configure(2, 'A', 1, state);
				break;
		case CAN0_RX_EN:
				TWI_Configure(2, 'A', 2, state);
				break;
		case CAN1_RX_EN:
				TWI_Configure(2, 'A', 3, state);
				break;
		case UART0_EN:
				TWI_Configure(2, 'A', 4, state);
				break;
		case UART0RTS_EN:
				TWI_Configure(2, 'A', 5, state);
				break;
		case UART0CTS_EN:
				TWI_Configure(2, 'A', 6, state);
				break;
		case UART0CTS_RTS_LPBK:
				TWI_Configure(2, 'A', 7, state);
				break;
		case UART0CTS_RST_EN:
				TWI_Configure(2, 'B', 0, state);
				break;
		case UART0CTS_146_EN:
				TWI_Configure(2, 'B', 1, state);
				break;
		case LED1_GPIO1_EN:
				TWI_Configure(2, 'B', 2, state);
				break;
		case LED1_GPIO2_EN:
				TWI_Configure(2, 'B', 3, state);
				break;
		case LED1_GPIO3_EN:
				TWI_Configure(2, 'B', 4, state);
				break;
		case PUSHBUTTON1_EN:
				TWI_Configure(2, 'B', 5, state);
				break;
		case PUSHBUTTON2_EN:
				TWI_Configure(2, 'B', 6, state);
				break;
        }
#endif

#ifdef __ADSPSC5xx__
	switch(signal)
	{
		case EEPROM_EN:
				TWI_Configure(1, 'A', 0, state);
				break;
		case UART0_FLOW_EN:
				TWI_Configure(1, 'A', 1, state);
				break;
		case UART0_EN:
				TWI_Configure(1, 'A', 2, state);
				break;
		case ETH0_EN:
				TWI_Configure(1, 'A', 3, state);
				break;
		case ETH1_EN:
				TWI_Configure(1, 'A', 4, state);
				break;
		case MLB3_EN:
				TWI_Configure(1, 'A', 5, state);
				break;
		case CAN0_EN:
				TWI_Configure(1, 'A', 6, state);
				break;
		case CAN1_EN:
				TWI_Configure(1, 'A', 7, state);
				break;
		case ADAU1962_EN:
				TWI_Configure(1, 'B', 0, state);
				break;
		case ADAU1979_EN:
				TWI_Configure(1, 'B', 1, state);
				break;
		case AUDIO_JACK_SEL:
				TWI_Configure(1, 'B', 2, state);
				break;
		case SD_WP_EN:
				TWI_Configure(1, 'B', 3, state);
				break;
		case SPI2_FLASH_CS_EN:
				TWI_Configure(1, 'B', 4, state);
				break;
		case SPI2_D2_D3_EN:
				TWI_Configure(1, 'B', 5, state);
				break;
		case SPDIF_OPTICAL_EN:
				TWI_Configure(1, 'B', 6, state);
				break;
		case SPDIF_DIGITAL_EN:
				TWI_Configure(1, 'B', 7, state);
				break;
		case PUSHBUTTON2_EN:
				TWI_Configure(2, 'A', 0, state);
				break;
		case PUSHBUTTON1_EN:
				TWI_Configure(2, 'A', 1, state);
				break;
		case LED1_EN:
				TWI_Configure(2, 'A', 2, state);
				break;
		case LED2_EN:
				TWI_Configure(2, 'A', 3, state);
				break;
		case LED3_EN:
				TWI_Configure(2, 'B', 4, state);
				break;
		
        }
#endif
}
/*****************************************************************************************
*	Function name:  TWI0_Init
*	Description:	This Function Enables the TWI.
*   Return value:    None.
*******************************************************************************************/


void TWI0_Init(void)
{
    *pREG_TWI0_CTL = 0x00;  					// Reset the TWI controller
	*pREG_TWI0_MSTRCTL 	= 0;
/**    *pREG_TWI0_MSTRSTAT = ( ENUM_TWI_MSTRSTAT_BUFWRERR_HI | ENUM_TWI_MSTRSTAT_BUFRDERR_HI |
    						ENUM_TWI_MSTRSTAT_DNAK_HI | ENUM_TWI_MSTRSTAT_ANAK_HI | ENUM_TWI_MSTRSTAT_LOSTARB_HI );**/  	// Clearing all status bits
	  *pREG_TWI0_MSTRSTAT = ( ENUM_TWI_MSTRSTAT_BUFWRERR_YES | ENUM_TWI_MSTRSTAT_BUFRDERR_YES |
	    						ENUM_TWI_MSTRSTAT_DNAK_YES | ENUM_TWI_MSTRSTAT_ANAK_YES | ENUM_TWI_MSTRSTAT_LOSTARB_YES );
	*pREG_TWI0_ISTAT 	=  	0xFF;				//	Clearing all pending interrupt flags
    *pREG_TWI0_FIFOCTL 	= ( ENUM_TWI_FIFOCTL_RXFLUSH | ENUM_TWI_FIFOCTL_TXFLUSH );
    //ssync();
	*pREG_TWI0_CTL		= 	BITM_TWI_CTL_EN | ((PRESCALE << BITP_TWI_CTL_PRESCALE) & BITM_TWI_CTL_PRESCALE);
	*pREG_TWI0_CLKDIV	= 	( ((CLKDIV_HI << BITP_TWI_CLKDIV_CLKHI) & BITM_TWI_CLKDIV_CLKHI) |
							  ((CLKDIV_LO << BITP_TWI_CLKDIV_CLKLO) & BITM_TWI_CLKDIV_CLKLO) );
//	*pREG_TWI0_FIFOCTL	= 	ENUM_TWI_FIFOCTL_RXILEN_2 | ENUM_TWI_FIFOCTL_TXILEN_2;
	*pREG_TWI0_FIFOCTL	= 	ENUM_TWI_FIFOCTL_RXTWOBYTE | ENUM_TWI_FIFOCTL_TXTWOBYTE;
	//__builtin_ssync();
}

/*****************************************************************************************
*	Function name:  TWI_Configure
*	Description:	This Function configures the TWI to enable the corresponding pin.
*		Parameter	 		|	Description		 					| Valid values      |
*	    I2C_index			|  										| 0 - 33			|
*		pin_number			|  										| 					|
*		state				|										|	0 or 1			|
*   Return value:    None.
*******************************************************************************************/
void TWI_Configure(int I2C_index,char port, char pin_number, int state)
{
	if (I2C_index == 0)
		{
			*pREG_TWI0_MSTRADDR = 0x20;
			if(port == 'A')
			{
					DATA_I2C_0_IODIR_A &= ~((1 << pin_number) & 0xFF);
				if (state)
					DATA_I2C_0_OLAT_A  |=  ((1 << pin_number) & 0xFF);
				else
					DATA_I2C_0_OLAT_A  &= ~((1 << pin_number) & 0xFF);

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_0_IODIR_A;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_0_IODIR_A;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_0_OLAT_A;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_0_OLAT_A;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();
			}
			else if (port == 'B')
			{
					DATA_I2C_0_IODIR_B &= ~((1 << pin_number) & 0xFF);
				if (state)
					DATA_I2C_0_OLAT_B  |=  ((1 << pin_number) & 0xFf);
				else
					DATA_I2C_0_OLAT_B  &= ~((1 << pin_number) & 0xFF);

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_0_IODIR_B;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_0_IODIR_B;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_0_OLAT_B;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_0_OLAT_B;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();
			}
		}

		else if (I2C_index == 1)
		{
			*pREG_TWI0_MSTRADDR = 0x21;
			if(port == 'A')
			{
					DATA_I2C_1_IODIR_A &= ~((1 << pin_number) & 0xFF);
				if (state)
					DATA_I2C_1_OLAT_A  |=  ((1 << pin_number) & 0xFF);
				else
					DATA_I2C_1_OLAT_A  &= ~((1 << pin_number) & 0xFF);

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_1_IODIR_A;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_1_IODIR_A;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_1_OLAT_A;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_1_OLAT_A;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();
			}
			else if (port == 'B')
			{
					DATA_I2C_1_IODIR_B &= ~((1 << pin_number) & 0xFF);
				if (state)
					DATA_I2C_1_OLAT_B  |=  ((1 << pin_number) & 0xFF);
				else
					DATA_I2C_1_OLAT_B  &= ~((1 << pin_number) & 0xFF);

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_1_IODIR_B;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_1_IODIR_B;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_1_OLAT_B;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_1_OLAT_B;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();
			}
		}

		else if (I2C_index == 2)
		{
			*pREG_TWI0_MSTRADDR = 0x22;
			if(port == 'A')
			{
					DATA_I2C_2_IODIR_A &= ~((1 << pin_number) & 0xFF);
				if (state)
					DATA_I2C_2_OLAT_A  |=  ((1 << pin_number) & 0xFF);
				else
					DATA_I2C_2_OLAT_A  &= ~((1 << pin_number) & 0xFF);

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_2_IODIR_A;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_2_IODIR_A;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_2_OLAT_A;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_2_OLAT_A;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();
			}
			else if (port == 'B')
			{
					DATA_I2C_2_IODIR_B &= ~((1 << pin_number) & 0xFF);
				if (state)
					DATA_I2C_2_OLAT_B  |=  ((1 << pin_number) & 0xFF);
				else
					DATA_I2C_2_OLAT_B  &= ~((1 << pin_number) & 0xFF);

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_2_IODIR_B;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_2_IODIR_B;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();

				*pREG_TWI0_TXDATA8 = 	ADDR_I2C_2_OLAT_B;
				*pREG_TWI0_TXDATA8 = 	DATA_I2C_2_OLAT_B;
				*pREG_TWI0_MSTRCTL =    DATA_CNT | BITM_TWI_MSTRCTL_EN;
				Wait_For_Completion();
			}
		}

//	__builtin_ssync();
}


/*****************************************************************************************
*	Function name:  Wait_For_Completion
*	Description:	This Function waits for the TWI transfers to complete.
*   Return value:    None.
*******************************************************************************************/
void Wait_For_Completion(void)
{
	int check_mcomp = 0;

	while(!check_mcomp)
	{
		check_mcomp  = 		*pREG_TWI0_ISTAT;
		check_mcomp &= 		BITM_TWI_ISTAT_MCOMP;

		//asm("ssync;");
	}

	*pREG_TWI0_ISTAT |= 	BITM_TWI_ISTAT_TXSERV;
		//asm("ssync;");

	*pREG_TWI0_ISTAT |= 	BITM_TWI_ISTAT_MCOMP;
		//asm("ssync;");
}


void TRU_Config(MasterID,SlaveID)
{
#define TRU_Config(MasterID,SlaveID) \
*pREG_TRU0_GCTL|=BITM_TRU_GCTL_EN;  \  
*pREG_TRU0_SSR##slaveID##=masterID;
}
