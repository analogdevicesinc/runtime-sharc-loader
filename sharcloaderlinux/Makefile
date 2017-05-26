GCC=arm-linux-gnueabi-gcc

PROC=sc589

BUILDROOT=/home/gabby/$(PROC)_dev/buildroot
INCLUDES=-I$(BUILDROOT)/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/usr/include/tirpc/ -I$(BUILDROOT)/linux/linux-kernel/drivers/staging/icc/include 
FLAGS=-mabi=aapcs-linux -mfpu=neon-vfpv4 -marm -pipe -mfloat-abi=hard -march=armv7-a -mcpu=cortex-a5 -std=gnu99 -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -pipe -Os


loadSharc: loader.c secdat_sh1.c secdat_sh2.c
	$(GCC) $(FLAGS) $(INCLUDES) -o $@ $^

.phony:  copy_to_tftp_dir
copy_to_tftp_dir: loadSharc
	sudo cp loadSharc /tftpboot

all: loadSharc copy_to_tftp_dir
