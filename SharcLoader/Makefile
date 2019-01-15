GCC=arm-linux-gnueabi-gcc

BUILDROOT_SC573=/home/wenting/sc5xx_dev/buildroot
BUILDROOT_SC589=/home/wenting/sc5xx_dev/buildroot
INCLUDES_SC589=-I$(BUILDROOT_SC589)/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/usr/include/tirpc/ -I$(BUILDROOT_SC589)/linux/linux-kernel/drivers/staging/icc/include 
INCLUDES_SC573=-I$(BUILDROOT_SC573)/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/usr/include/tirpc/ -I$(BUILDROOT_SC573)/linux/linux-kernel/drivers/staging/icc/include 
FLAGS=-mabi=aapcs-linux -mfpu=neon-vfpv4 -marm -pipe -mfloat-abi=hard -march=armv7-a -mcpu=cortex-a5 -std=gnu99 -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -pipe -Os

all: exec_all

SC589: loader.c secdat_SC589_sh1.c secdat_SC589_sh2.c
	$(GCC) $(FLAGS) $(INCLUDES_SC589) -D SC589 -o loadsharc_$@ $^
	
SC573: loader.c secdat_SC573_sh1.c secdat_SC573_sh2.c
	$(GCC) $(FLAGS) $(INCLUDES_SC573) -D SC573 -o loadsharc_$@ $^

exec_all: SC589 SC573
