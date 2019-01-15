/*****************************************************************************
 * LED_BLINK_SC573_Core2.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include <string.h>
#include "adi_initialize.h"
#include "LED_BLINK_SC573_Core2.h"
#include "recv_arg.h"

//PORT A
volatile uint32_t *pREG_PORT_FER_CLR_A = pREG_PORTA_FER_CLR;
volatile uint32_t *pREG_PORT_DIR_SET_A = pREG_PORTA_DIR_SET;
volatile uint32_t *pREG_PORT_DATA_TGL_A= pREG_PORTA_DATA_TGL;

//PORT C
volatile uint32_t *pREG_PORT_FER_CLR_C = pREG_PORTC_FER_CLR;
volatile uint32_t *pREG_PORT_DIR_SET_C = pREG_PORTC_DIR_SET;
volatile uint32_t *pREG_PORT_DATA_TGL_C= pREG_PORTC_DATA_TGL;
volatile uint32_t *pREG_PORT_DATA_CLR_C= pREG_PORTC_DATA_CLR;

//PORT D
volatile uint32_t *pREG_PORT_FER_CLR_D = pREG_PORTD_FER_CLR;
volatile uint32_t *pREG_PORT_DIR_SET_D = pREG_PORTD_DIR_SET;
volatile uint32_t *pREG_PORT_DATA_TGL_D= pREG_PORTD_DATA_TGL;

//PORT E
volatile uint32_t *pREG_PORT_FER_CLR_E = pREG_PORTE_FER_CLR;
volatile uint32_t *pREG_PORT_DIR_SET_E = pREG_PORTE_DIR_SET;
volatile uint32_t *pREG_PORT_DIR_CLR_E = pREG_PORTE_DIR_CLR;
volatile uint32_t *pREG_PORT_DATA_TGL_E= pREG_PORTE_DATA_TGL;
volatile uint32_t *pREG_PORT_DATA_CLR_E= pREG_PORTE_DATA_CLR;

//PORT F
volatile uint32_t *pREG_PORT_FER_CLR_F = pREG_PORTF_FER_CLR;
volatile uint32_t *pREG_PORT_DIR_SET_F = pREG_PORTF_DIR_SET;
volatile uint32_t *pREG_PORT_DATA_TGL_F= pREG_PORTF_DATA_TGL;

#define TIME_DELAY 0x5FFFFF

#define PORT_A 0x0200
#define PORT_C 0x0010
#define PORT_D 0x0040
#define PORT_E 0xA002
#define PORT_F 0x0800

void main(int argc, char *argv[])
{
	/* Receive arguments from the RSL */
	recv_arg(&argc, &argv);

	/**
	 * Initialize managed drivers and/or services that have been added to
	 * the project.
	 * @return zero on success
	 */
	// By default, the library requires the Core 1 to run prior to the
	// Core 2. This demo project doesn't rely on the libraries, in order
	// to demo the Core 2 running independently, the library initialization
	// is temporarily disabled.
	//adi_initComponents();

	uint32_t var[2];

	/* Begin adding your custom code here */

	/* For the reference:
	GPIO LEDs
	Reference Designator Programmable Flag Pin
	LED10 PE_13
	LED11 PA_09
	LED12 PE_01
	LED13 PC_14
	LED14 PC_04
	LED15 PF_11
	LED16 PD_06
	LED17 PE_15  */

	*pREG_PORT_FER_CLR_A      = PORT_A;       // GPIO modes
	*pREG_PORT_DIR_SET_A  = PORT_A;  // pin 09 is output

	*pREG_PORT_FER_CLR_C      = PORT_C;       // GPIO modes
	*pREG_PORT_DIR_SET_C  = PORT_C;  // pin 14 is output

	*pREG_PORT_FER_CLR_D      = PORT_D;       // GPIO modes
    *pREG_PORT_DIR_SET_D  = PORT_D;  // pin 6 is output

	*pREG_PORT_FER_CLR_E      = PORT_E;       // GPIO modes
	*pREG_PORT_DIR_SET_E  = PORT_E;  // pins 13,1 are output

	*pREG_PORT_FER_CLR_F      = PORT_F;       // GPIO modes
	*pREG_PORT_DIR_SET_F  = PORT_F;  // pin 11 is output

	// From command line, determine which LED to be light up.
	uint32_t leds = 0;
	for (int i = 0; i < argc; i++) {
		if      (strcmp(argv[i], "A") == 0)	leds |= 0x01;
		else if (strcmp(argv[i], "C") == 0)	leds |= 0x02;
		else if (strcmp(argv[i], "D") == 0)	leds |= 0x04;
		else if (strcmp(argv[i], "E") == 0)	leds |= 0x08;
		else if (strcmp(argv[i], "F") == 0)	leds |= 0x10;
	}

	int i;

	while(1)
	{
		*pREG_PORT_DATA_TGL_A  = (leds & 0x01) ? PORT_A : 0;   // SC573 ezboard
		*pREG_PORT_DATA_TGL_C  = (leds & 0x02) ? PORT_C : 0;   // SC573 ezboard
		*pREG_PORT_DATA_TGL_D  = (leds & 0x04) ? PORT_D : 0;   // SC573 ezboard
		*pREG_PORT_DATA_TGL_E  = (leds & 0x08) ? PORT_E : 0;   // SC573 ezboard
		*pREG_PORT_DATA_TGL_F  = (leds & 0x10) ? PORT_F : 0;   // SC573 ezboard

		// delay loop
		for(i=0; i<TIME_DELAY; i++);

		var[0] = 0xdead;
		var[1] = 0xbeef;
	}

}
