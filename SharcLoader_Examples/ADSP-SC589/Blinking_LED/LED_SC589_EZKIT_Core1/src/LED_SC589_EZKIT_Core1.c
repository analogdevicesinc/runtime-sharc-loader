/*****************************************************************************
 * LED_SC589_EZKIT_Core1.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include <string.h>
#include "adi_initialize.h"
#include "LED_SC589_EZKIT_Core1.h"
#include "recv_arg.h"

#define TIME_DELAY 0x5FFFFF

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
	
	/* Begin adding your custom code here */

	uint32_t var[2];
	uint32_t leds;

	leds = 0;
	for (int i = 0; i < argc; i++) {
		if      (strcmp(argv[i], "1") == 0)	leds |= 0x8000;
		else if (strcmp(argv[i], "2") == 0)	leds |= 0x4000;
		else if (strcmp(argv[i], "3") == 0)	leds |= 0x2000;
	}


	*pREG_PORTE_FER      = 0;       // GPIO mode
	*pREG_PORTE_DIR_SET  = 0xe000;  // Pins 15:13 are output
	*pREG_PORTE_DATA_CLR = 0xe000;  // Clear Pins 15:13

	int i;
	while(1)
	{

		*pREG_PORTE_DATA_TGL  = leds;               // SC589 ezkit

		for(i=0; i<TIME_DELAY; i++);

		var[0] = 0xdead;
		var[1] = 0xbeef;

	}

	return 0;
}

