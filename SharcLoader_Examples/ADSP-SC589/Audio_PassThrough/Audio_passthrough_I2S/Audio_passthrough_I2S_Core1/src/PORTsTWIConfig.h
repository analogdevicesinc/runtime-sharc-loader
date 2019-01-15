/***************************************************************************************
* 	File name : PortsTWIConfig.h
* 	Purpose: Includes the macros, variables and function declarations for PortsTWIConfig.C
* 	Author: Vinod B
* 	VDSP version used for testing: Andromeda Beta(Oct 14th 2011)
* 	Hardware used: BF609 EZ-KIT
* 	Connection details :  N/A
* 	Guidelines for change: N/A
*****************************************************************************************/
#ifndef PORTSTWICONFIG_H_
#define PORTSTWICONFIG_H_

#include "ProcInclude.h"

// Function Declaration
void Switch_Enable(int signal, int state);
void TWI_Configure(int I2C_index,char port, char pin_number, int state);
void TWI0_Init(void);
void Wait_For_Completion(void);

// Macros
#define PRESCALE 12				//	TWI

#define 	CLKDIV_HI 		0xC
#define 	CLKDIV_LO 		0xD
//========================================
#define ADDR_I2C_0_IODIR_A	0x00
#define ADDR_I2C_0_OLAT_A	0x14
#define ADDR_I2C_0_IODIR_B	0x01
#define ADDR_I2C_0_OLAT_B	0x15

//========================================
#define ADDR_I2C_1_IODIR_A	0x00
#define ADDR_I2C_1_OLAT_A 	0x14
#define ADDR_I2C_1_IODIR_B	0x01
#define ADDR_I2C_1_OLAT_B	0x15

//========================================
#define ADDR_I2C_2_IODIR_A	0x00
#define ADDR_I2C_2_OLAT_A	0x14
#define ADDR_I2C_2_IODIR_B	0x01
#define ADDR_I2C_2_OLAT_B	0x15


#define Data_count 2
#define DATA_CNT ( Data_count << 6)

#ifdef __ADSPBF609__
//signal_name                               value to be passed
#define POS_CAN_EN   		 			0
#define POS_CAN_STB 		  	  		1
#define POS_CAN0_ERR_EN			  		2
#define POS_CAN0RX_EN 					3
#define	POS_CNT0UD_EN					4
#define POS_CNT0DG_EN 				 	5
#define POS_CNT0ZM_EN 				  	6
#define POS_RMII_CLK_EN   		  	  	7
#define POS_UART0_RTS_EN 				8
#define POS_UART0RX_EN	   			  	9
#define POS_UART0CTS_EN				  	10
#define POS_UART0CTS_RTS_LPBK		        	11
#define POS_UART0CTS_RST_EN 		        	12
#define POS_UART0CTS_146_EN			  	13
#define POS_TEMP_IRQ_EN 				14
#define POS_TEMP_THERM_EN				15
#define POS_OVERRIDE_EBIU_LP0_BOOT	        	16
#define POS_EBIU_EPPI2_LP1_SWITCH		        17
#define POS_EBIU_LP0_EN			         	18
#define POS_LED1_GPIO1_EN 			  	19
#define POS_LED1_GPIO2_EN 			  	20
#define POS_LED1_GPIO3_EN				21
#define POS_LED1_GPIO4_EN				22
#define POS_PUSHBUTTON1_EN 			  	23
#define POS_PUSHBUTTON2_EN 			  	24
#define POS_SD_CD_EN				        25
#define POS_SD_WP_EN 					26
#define POS_SPIFLASH_CS_EN			  	27
#define POS_SPI0D2_EN 				  	28
#define POS_SPI0D3_EN 				  	29
#define POS_PHYINT_EN 				  	30
#define POS_PHY_PWR_DWN_INT			  	31
#define POS_PHYAD0 				  	32
#define POS_ETHERNET_DISABLE 		        	33
#endif

#ifdef __ADSPCM40X__
//signal_name                                value to be passed
#define POS_SPIFLASH_CS_EN   				0
#define POS_SPI0D2_EN 		  	  		1
#define POS_SPI0D3_EN			  		2
#define POS_UART0RTS_EN					3
#define	POS_UART0RX_EN  				4
#define POS_UART0CTS_EN 			 	5
#define POS_UART0CTS_RTS_LPBK			  	6
#define POS_UART0CTS_RTS_EN   		  	  	7
#define POS_CAN0_EN     				8
#define POS_PUSHBUTTON1_EN    			  	9
#define POS_PUSHBUTTON2_EN         		  	10
#define POS_LED1_GPIO1_EN		        	11
#define POS_LED2_GPIO2_EN 		        	12
#define POS_LED3_GPIO3_EN			  	13
#define POS_LED4_GPIO4_EN 				14
//#define POS_CNT0UD_EN    				15
//#define POS_CNT0DG_EN             	        	16
//#define POS_CNT0ZM_EN            		        17
#define POS_RS485_RE			         	15  //18
#define POS_RS485_DE     			  	16  //19
#define POS_CAN1_EN     			  	17  //20
#define POS_ETH0_MDIO_BUS_EN				18  //21
#define POS_ETHERNET_EN              			19  //22
#define POS_PHYINT_EN    			  	20  //23
#define POS_RMII_CLK_EN 			  	21  //24
#define POS_WAKE_PUSHBUTTON_EN 			        22  //25
#define DATA_LOW_EXP_EN                                 23
#define DATA_HIGH_EXP_EN                                24
#define TRACE_EXP_EN                                    25
#define PB_08_09_EXP_EN                                 26
#define ETH_EXP_EN                                      27
#endif

#ifdef __ADSPBF707__
//signal_name                               value to be passed
#define SPIFLASH_CS_EN   		 			0
#define SPIFLASH_D2_EN 		  	  		1
#define SPIFLASH_D3_EN			  		2
#define SD_CD_EN 					3
#define	SD_WP_EN					4
#define CAN0_EN 				 	5
#define CAN0_STB 				  	6
#define CAN1_EN   		  	  	7
#define CAN1_STB 				8
#define CAN0_ERR_EN	   			  	9
#define CAN1_ERR_EN				  	10
#define CAN0_TX_EN		        	11
#define CAN1_TX_EN 		        	12
#define CAN0_RX_EN			  	13
#define CAN1_RX_EN 				14
#define UART0_EN				15
#define UART0RTS_EN	        	16
#define UART0CTS_EN		        17
#define UART0CTS_RTS_LPBK			         	18
#define UART0CTS_RST_EN 			  	19
#define UART0CTS_146_EN 			  	20
#define LED1_GPIO1_EN				21
#define LED1_GPIO2_EN				22
#define LED1_GPIO3_EN 			  	23
#define PUSHBUTTON1_EN 			  	24
#define PUSHBUTTON2_EN				        25
#endif

#ifdef __ADSPSC5xx__
//signal_name                               value to be passed
#define EEPROM_EN   		 			0
#define UART0_FLOW_EN 		  	  		1
#define UART0_EN			  		2
#define ETH0_EN 					3
#define	ETH1_EN					4
#define MLB3_EN 				 	5
#define CAN0_EN 				  	6
#define CAN1_EN   		  	  	7
#define ADAU1962_EN 				8
#define ADAU1979_EN	   			  	9
#define AUDIO_JACK_SEL				  	10
#define SD_WP_EN		        	11
#define SPI2_FLASH_CS_EN 		        	12
#define SPI2_D2_D3_EN			  	13
#define SPDIF_OPTICAL_EN 				14
#define SPDIF_DIGITAL_EN				15
#define PUSHBUTTON2_EN	        	16
#define PUSHBUTTON1_EN		        17
#define LED1_EN			         	18
#define LED2_EN 			  	19
#define LED3_EN 			  	20
#endif



#endif

