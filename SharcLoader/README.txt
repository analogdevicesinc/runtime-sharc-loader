EE-399  Using the Linux Runtime SHARC+ Loader on ADSP-SC58x and ADSP-SC57x Processors

Runtime Sharc Loader

This linux application reads in a SHARC application loader file and boots it onto one of the SHARC cores.

files:
-------------------
loader.c - main application 

secdat_SC573_sh1.c - loader stub that will be loaded onto ADSP-SC573 sharc core 1
secdat_SC573_sh2.c - loader stub that will be loaded onto ADSP-SC573 sharc core 2
secdat_SC589_sh1.c - loader stub that will be loaded onto ADSP-SC589 sharc core 1
secdat_SC589_sh2.c - loader stub that will be loaded onto ADSP-SC589 sharc core 2

LED_SC589_EZKIT_Core1.ldr - LED blink application that runs on ADSP-SC589 core 1
LED_SC589_EZKIT_Core2.ldr - LED blink application that runs on ADSP-SC589 core 2
LED_BLINK_SC573_Core1.ldr - LED blink application that runs on ADSP-SC573 core 1
LED_BLINK_SC573_Core2.ldr - LED blink application that runs on ADSP-SC573 core 2
Audio_passthrough_I2S_589_Core1.ldr - talktru program that runs on sharc core 1

usage:
-------------------
Below is the help message from the RSL (loadSharc).  A binary boot stream LDR file is specified along with
which core to boot.  By default, core 1 will be booted if no core is specified.  Optionally, the user can
specify a debug level to indicate the verbosity level of printed output and debug messages.

# for ADSP-SC589 
./loadsharc_SC589 -help
usage:
  loadsharc_SC589 [options]
options:
  -i [LDR file]           Sharc Binary LDR file to be
  -s [Sharc Core #]       Sharc Core to load (1 or 2)
  -d [debug level]    Amount of debug info 1, 2 or 3
  -a [Arguments]      Arguments that will be passed to SHARC
  -h                  Show this help message
  
# for ADSP-SC573
./loadsharc_SC589 -help
usage:
  loadsharc_SC589 [options]
options:
  -i [LDR file]           Sharc Binary LDR file to be
  -s [Sharc Core #]       Sharc Core to load (1 or 2)
  -d [debug level]    Amount of debug info 1, 2 or 3
  -a [Arguments]      Arguments that will be passed to SHARC
  -h                  Show this help message
  
# MakeFile
make ADSP-SC589 - to compile for ADSP-SC589
make ADSP-SC573 - to compile for ADSP-SC573
make all - compile ADSP-SC589 and ADSP-SC573 at same time




