/**************************************************************************************************************************
*	File name 							: ProcInclude.h
*	Purpose								: Includes the header files specific to the processor name.
*	Author								: Mitesh Moonat
*	VDSP version used for testing		: CCES Cygnus M2
*	Hardware used						: N/A
*	Connection details 					: N/A
*	Guidelines for change				: N/A
**************************************************************************************************************************/

#ifndef PROCINCLUDE_H_
#define PROCINCLUDE_H_
#if defined (__ADSPBF702__)
	#include <defBF702.h>
	#include <cdefBF702.h>
#elif defined (__ADSPBF703__)
	#include <defBF703.h>
	#include <cdefBF703.h>
#elif defined (__ADSPBF704__)
	#include <defBF704.h>
	#include <cdefBF704.h>
#elif defined (__ADSPBF705__)
	#include <defBF705.h>
	#include <cdefBF705.h>
#elif defined (__ADSPBF706__)
	#include <defBF706.h>
	#include <cdefBF706.h>
#elif defined (__ADSPBF707__)
	#include <defBF707.h>
	#include <cdefBF707.h>

#elif defined (__ADSPSC589__)
	#include <defSC589.h>
	#include <cdefSC589.h>
#elif defined (__ADSPBF609__)
	#include <defBF609.h>
	#include <cdefBF609.h>
#else
	#error Processor Type Not Supported
#endif

#endif /* PROCINCLUDE_H_ */
