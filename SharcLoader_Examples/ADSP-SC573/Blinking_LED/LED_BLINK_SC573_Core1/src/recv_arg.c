/*
 * recv_arg.c
 */
#include <stdint.h>
#include <stdlib.h>
#include <sys/platform.h>

// For SC589
//#define L2_START 0x20080000
// For SC573
#define L2_START 0x20000000

#define L2_MCAPI_SH1_OFFSET (12*1024)
#define L2_MCAPI_SH2_OFFSET (8*1024)
#define L2_MCAPI_SH1_START (L2_START + L2_MCAPI_SH1_OFFSET)
#define L2_MCAPI_SH2_START (L2_START + L2_MCAPI_SH2_OFFSET)

#ifdef CORE1
#define L2_MCAPI_START L2_MCAPI_SH1_START
#elif (defined CORE2)
#define L2_MCAPI_START L2_MCAPI_SH2_START
#endif

void recv_arg(int *argc, char **argv[]) {
	// The mailbox is located in the L2 memory
	// Though it will not be used afterwards, the user still should be cautious
	// not to use this area for static non-zero variable storage. It is safe
	// to use as zero initialized variable storage , heap or stack storage.
	volatile uint32_t *ptrMboxSh2Arm_l2 = (uint32_t *)L2_MCAPI_START;
	volatile uint32_t *ptrMboxArm2Sh_l2 = ptrMboxSh2Arm_l2 + 1;
	volatile uint32_t *ptrDataSh2Arm_l2 = ptrMboxSh2Arm_l2 + 2;
	volatile uint32_t *ptrDataArm2Sh_l2 = ptrMboxSh2Arm_l2 + 3;

	uint32_t cmdline_length;
	volatile char *cmdline;
	volatile char *p;
	int arg_count;
	char * *arg_value;

	// Tell the ARM that the SHARC is running
	*ptrMboxSh2Arm_l2 = 0x5555;
	// Waiting ack and fetch length from ARM
	while (*ptrMboxArm2Sh_l2 != 0x5555);
	cmdline_length = *ptrDataArm2Sh_l2;

	// Try to allocate the memory and send back the address
	cmdline = malloc(cmdline_length);
	if (cmdline == NULL) {
		*ptrMboxSh2Arm_l2 = 0xdead;
		// We still need to wait the ARM to confirm the error
		// So we do not clean the mailbox value too early
		while (*ptrMboxArm2Sh_l2 != 0xbeef);
		goto _clean;
	}
	*ptrDataSh2Arm_l2 = (uint32_t)cmdline;
	*ptrMboxSh2Arm_l2 = 0xbbbb;

	// Wait till ARM has finished the transmission
	while(*ptrMboxArm2Sh_l2 != 0x6666);

	// Parse the string
	// First step is determine the count of arguments
	// So the memory for argv (pointer array) can be allocated
	p = cmdline;
	if (*p) {
		arg_count = 1; // non empty, at least one
		while (*p) {
			if (*p++ ==' ') arg_count ++;
		}
	}
	else {
		arg_count = 0;
	}

	if (arg_count != 0) {
		p = cmdline;
		arg_value = malloc(arg_count * sizeof(char *));
		arg_count = 0;
		arg_value[arg_count ++] = (char *)p;
		while (*p) {
			if (*p==' ') {
				arg_value[arg_count ++] = (char *)p + 1;
				*p = '\0';
			}
			p++;
		}
		*argc = arg_count;
		*argv = arg_value;
	}
	else {
		// No arguments, return 0.
		*argc = 0;
		*argv = NULL;
	}

_clean:
	// Intentionally not freeing the cmdline
	// So the user can use the arguments in the application
	*ptrMboxSh2Arm_l2 = 0x0000;
	*ptrMboxArm2Sh_l2 = 0x0000;
	*ptrDataSh2Arm_l2 = 0x0000;
	*ptrDataArm2Sh_l2 = 0x0000;
	return;
}

