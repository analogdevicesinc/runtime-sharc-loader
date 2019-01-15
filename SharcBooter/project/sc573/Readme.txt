EE-399 Using the Linux Runtime SHARC+ Loader on ADSP-SC589 and ADSP-SC573 Processors

Loader Stub for ADSP-SC573 SHARC core 1 and 2

This SHARC application performs a "memory boot" from a memory buffer. It uses the boot kernel API to boot from run-time. It supplies and registers a "Load" function for this "memory boot". Since this application is meant to work in conjunction with a Linux application, loadSharc, the "Load" function communicates with loadSharc to request boot stream data and wait for it.

This has been developed and verified on:

CCES 2.8.0 on an ADSP-SC573 EZ-BOARD 1.0 (0.0 silicon)

#############Latest Changes###################
This version adds the arguments passing function, moves the loader stub into L2, and enables the L1
parity check.

Verified on
CCES 2.8.1 on an ADSP-SC573 EZ-BOARD 1.5 (0.0 silicon)
