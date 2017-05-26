/*
** ADSP-SC589 startup code generated on Apr 19, 2017 at 15:23:53.
*/
/*
** Copyright (C) 2000-2016 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically based upon the options selected
** in the System Configuration utility. Changes to the Startup Code configuration
** should be made by modifying the appropriate options rather than editing
** this file. To access the System Configuration utility, double-click the
** system.svc file from a navigation view.
**
** Custom additions can be inserted within the user-modifiable sections,
** these are bounded by comments that start with "$VDSG". Only changes
** placed within these sections are preserved when this file is re-generated.
**
** Product      : CrossCore Embedded Studio
** Tool Version : 6.0.6.0
*/

.FILE_ATTR libGroup="startup";
.FILE_ATTR libName="libc";

.FILE_ATTR libFunc="___lib_prog_term";
.FILE_ATTR FuncName="___lib_prog_term";
.FILE_ATTR libFunc="start";
.FILE_ATTR FuncName="start";
.FILE_ATTR prefersMem="any";
.FILE_ATTR prefersMemNum="50";

#include <sys/anomaly_macros_rtl.h> // defines silicon anomaly macros
#include <interrupt.h>              // defines interrupt support
#include <platform_include.h>       // defines MMR macros
#include <adi_osal.h>               // OSAL support
#include <sys/fatal_error_code.h>   // defines fatal error support

/* Suppress the assemblers informational message regarding the automatic
** call sequence change it does when building -swc.
*/
.MESSAGE/SUPPRESS 2536;


.GLOBAL start;

.SECTION/CODE/DOUBLEANY seg_pmco;

start:

      /* Enable instruction, data and system transfer parity checking for
      ** parts and revisions that have all required support. The ADSP-SC58x
      ** and ADSP-2158x revisions 0.0 and 0.1 system reset does not work and
      ** this is required so we don't enable parity for these.
      */
#if !defined(__ADSPSC589_FAMILY__) || \
     (!defined(__SILICON_REVISION__) || \
      ((__SILICON_REVISION__ >= 0x0100) && (__SILICON_REVISION__ != 0xFFFF)))
      BIT SET MODE1 (BITM_REGF_MODE1_IPERREN | BITM_REGF_MODE1_DPERREN |
                     BITM_REGF_MODE1_SPERREN );
      /* Wait 11 cycles for the this to take effect. */
      NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
      //#define ENABLED_PARITY_ERROR_CHECKING
#endif

      R0 = DM(REG_SHBTB0_CFG);              /* Get current BTB configuration. */
      R0 = BCLR R0 BY BITP_SHBTB_CFG_DIS;   /* Enable the BTB                 */
      R0 = BSET R0 BY BITP_SHBTB_CFG_INVAL; /* Invalidate the BTB             */
      /* Turn off software return prediction in the BTB, because we can't
      ** guarantee i12 has a valid code address, and a speculative access
      ** to an invalid address can cause a hardware error.
      */
      R0 = BSET R0 BY BITP_SHBTB_CFG_SRETDIS; /* Software return disable.     */
      DM(REG_SHBTB0_CFG) = R0;              /* Write back to BTB_CFG MMR.     */

      /* Wait 11 cycles for the invalidation of the BTB to take effect. */
      NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;

      /* Set the vector register to 'start' so that if the processor
      ** is reset (for example, when the ARM calls adi_core_enable for the
      ** SHARCs), the processor bypasses the boot ROM code.
      */
      R0 = start;
      DM(REG_RCU0_SVECT2) = R0;

#if 1
.WAITLOOP:
      // Wait for this core to be enabled.
      R0 = DM(REG_RCU0_MSG);
      BTST R0 BY BITP_RCU_MSG_C2ACTIVATE;
      IF SZ JUMP .WAITLOOP;
#endif

/*$VDSG<insert-code-very-beginning>                             */
.start_of_user_code_very_beginning:
      /*
      ** Insert any additional code to be executed before any other
      ** startup code here.
      ** Code inserted here is preserved when this file is re-generated.
      ** Note that the C runtime environment hasn't been set up yet
      ** so you cannot call C or C++ functions.
      */
.end_of_user_code_very_beginning:
/*$VDSG<insert-code-very-beginning>                             */

      /*
      ** Initializes the processor, memory, C runtime and heaps.
      */
      .EXTERN ___lib_setup_c;
      CALL ___lib_setup_c;


/*$VDSG<insert-code-after-setup>                                */
.start_of_user_code_after_setup:
      /*
      ** Insert any additional code to be executed at the start after
      ** the C runtime and hardware has been initialized.
      ** Code inserted here is preserved when this file is re-generated.
      */
.end_of_user_code_after_setup:
/*$VDSG<insert-code-after-setup>                                */

      /*
      ** Enable the ILOPI interrupt to support illegal opcode detection.
      ** The interrupt vector code for Illegal Opcode detection
      ** jumps directly to stub handler _adi_ilop_detected.
      */
      .EXTERN adi_rtl_activate_dispatched_handler.;
      R4 = ADI_CID_ILOPI;
      CJUMP adi_rtl_activate_dispatched_handler. (DB);
         DM(I7, M7) = R2;
         DM(I7, M7) = PC;

#if defined(ENABLED_PARITY_ERROR_CHECKING)
      /*
      ** Enable the PARI interrupt to support L1 parity error detection.
      ** The interrupt vector code for this interrupt jumps directly
      ** to stub handler _adi_parity_error_detected.
      */
      .EXTERN adi_rtl_activate_dispatched_handler.;
      R4 = ADI_CID_PARI;
      CJUMP adi_rtl_activate_dispatched_handler. (DB);
         DM(I7, M7) = R2;
         DM(I7, M7) = PC;
#endif

      /*
      ** Call the OSAL init function.
      */
      .EXTERN adi_osal_Init.;    // ADI_OSAL_STATUS adi_osal_Init(void);
      CJUMP adi_osal_Init. (DB);
         DM(I7, M7) = R2;
         DM(I7, M7) = PC;

      R1 = E_ADI_OSAL_SUCCESS;
      COMPU(R0, R1);
      IF NE JUMP .osal_Init_failed;

      R4 = R4 - R4,              // argc = 0
         R8 = M5;                // argv = NULL


/*$VDSG<insert-code-before-call-to-main>                        */
.start_of_user_code1:
      /*
      ** Insert any additional code to be executed before calling main() here.
      ** Code inserted here is preserved when this file is re-generated.
      ** Avoid clobbering the values of registers R4 and R8 as they hold
      ** the argc and argv parameters for main().
      */
.end_of_start_of_user_code1:
/*$VDSG<insert-code-before-call-to-main>                        */

      .EXTERN main.;
      JUMP main. (DB);           // call main()
         DM(I7, M7) = 0;         // NULL FP to terminate call stack unwinding
         DM(I7, M7) = PC;

      .GLOBAL ___lib_prog_term;
___lib_prog_term:
#if WA_20000009
      /* If there's a memory access in the previous 5 cycles, we might hit
      ** the 20000009 anomaly. However, it's unlikely, because:
      ** - we should only jump here once, so the jump won't be predicted
      ** - the jumps to here from library code don't have delay slots.
      ** So suppress the anomaly warning.
      */
      .MESSAGE/SUPPRESS 2561 FOR 2 LINES;
#endif
      IDLE;
      JUMP ___lib_prog_term;     // Stay put.
.___lib_prog_term.end:

      /*
      ** The call to _adi_osal_Init returned an error so call _adi_fatal_error.
      */
      .EXTERN adi_fatal_error.;
.osal_Init_failed:
      R12 = R0;                  // pass adi_osal_Init result value
      JUMP adi_fatal_error. (DB); // doesn't return
         R4 = _AFE_G_LibraryError;
         R8 = _AFE_S_adi_osal_Init_failure;
.osal_Init_failed.end:

.start.end:

