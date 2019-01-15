#!/usr/bin/bash

#cygwin 


ELFDUMP="/cygdrive/c/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.8.1/elfdump.exe"

SHARCCORE=$2



# get section numbers that have opcode and convert them to decimal
eval $ELFDUMP $1 | grep progbits | sed -e '/ progbits 00000000 00000000/d' -e '/ progbits.*00000000:/d' -e 's/ progbits.*//' | { while read a; do echo $((16#$a)); done } > idx.tmp

# get the size of each section
eval $ELFDUMP $1 | grep progbits | sed -e '/ progbits 00000000 00000000/d' -e 's/[0-9a-f]*  progbits [0-9a-f]* [0-9a-f]* [0-9a-f]* \([0-9a-f]*\).*/0x\1/g' -e '/0x00000000/d'> sectszs.tmp

# dump out section data with opcodes 
cat idx.tmp | { while read a; do eval $ELFDUMP -ix $a $1; done } > dump1.tmp

# get beginning addresses of each section
#  todo: double check correctness of awk command
awk '/---/, /[0-9a-f]{8}:/' dump1.tmp | sed 's/^\([0-9a-f]*\):.*/\t0x\1,/g' | sed "s/----.*\[\([0-9]*\)\]/sect\1/" | tr '\n' ' ' | sed "s/\(sect[0-9]*\) ----\s*0x\([0-9a-f]*\),/\t0x\2, sh${SHARCCORE}_\1,\n/g" > tgts1.tmp

# add in the section sizes
cat tgts1.tmp | { i=1; while read a; do echo $a `sed -n "$i p" sectszs.tmp`,; ((i++)); done } > tgts2.tmp

# start creating source code file
printf "/* This is a generated file processed from dxe file */\n" > xyz.c
echo   "/* Generated on" `date` "*/" >> xyz.c
printf "\n#include <sys/platform.h>\n\ntypedef struct appLdrSect{\n\tuint32_t tgt;\n\tuint8_t *sect;\n\tuint32_t sectsz;\n} appLdrSect_t;\n\n" >> xyz.c 


# format dump of sections into data buffers
sed -e 's/----.*\[\([0-9]*\)\] ----/uint8_t sect\1[]={/' -e 's/.*://' -e 's/\([0-9a-f]\) \([0-9a-f]\)/\1\2/g' -e 's/\([0-9a-f][0-9a-f]\)/0x\1, /g' -e  "s/s0xec, t0x\([0-9a-f]*\), /sh${SHARCCORE}_sect\1/" -e  "s/s0xec, t\([0-9a-f]\)/sh${SHARCCORE}_sect\1/" < dump1.tmp | tr '\n' ' ' | sed -e 's/\s*//g' -e 's/uint8_t/\n\nuint8_t /g' -e 's/,/, /g' > dump2.tmp


# add new line at end of file and then add closing brace to buffers
printf '\n' | cat dump2.tmp - | sed 's/,\s*$/};/g' | cat xyz.c - > abc.c


printf "\nappLdrSect_t mySectsSharc$SHARCCORE[]={\n" | cat abc.c - tgts2.tmp > z

printf "};\n" | cat z - > y

NS=_numsects
printf "\nuint32_t sh$SHARCCORE$NS = `wc -l idx.tmp | sed 's/^\([0-9]*\).*/\1/'`;\n" | cat y - > secdat.c

PS=_progStartAddr
printf "uint32_t sh$SHARCCORE$PS = " >> secdat.c

eval $ELFDUMP -all $1 | grep "'start'" | grep function |  grep function | sed -e 's/[0-9a-f]* [0-9a-f]* \([0-9a-f]*\) .*/0x\1;\n/' >> secdat.c

printf "volatile uint32_t *ptrMboxSh2Arm_sh$SHARCCORE = (uint32_t *)" >> secdat.c

eval $ELFDUMP -all $1 | grep "'mailbox2arm'" | grep object | sed -e 's/[0-9a-f]* [0-9a-f]* \([0-9a-f]*\) .*/0x\1;\n/' >> secdat.c

printf "volatile uint32_t *ptrMboxArm2Sh_sh$SHARCCORE = (uint32_t *)" >> secdat.c

eval $ELFDUMP -all $1 | grep "'mailbox2sharc'" | grep object | sed -e 's/[0-9a-f]* [0-9a-f]* \([0-9a-f]*\) .*/0x\1;\n/' >> secdat.c







printf "volatile uint32_t *ptrLdSync2ArmByteCount_sh$SHARCCORE = (uint32_t *)" >> secdat.c

eval $ELFDUMP -all $1 | grep "'ldSync2ArmByteCount'" | grep object | sed -e 's/[0-9a-f]* [0-9a-f]* \([0-9a-f]*\) .*/0x\1;\n/' >> secdat.c

printf "volatile uint32_t *ptrLdSync2ShByteCount_sh$SHARCCORE = (uint32_t *)" >> secdat.c

eval $ELFDUMP -all $1 | grep "'ldSync2ShByteCount'" | grep object | sed -e 's/[0-9a-f]* [0-9a-f]* \([0-9a-f]*\) .*/0x\1;\n/' >> secdat.c

printf "volatile uint32_t *ptrBootStream_sh$SHARCCORE = (uint32_t *)" >> secdat.c

eval $ELFDUMP -all $1 | grep "'bootstream'" | grep object | sed -e 's/[0-9a-f]* [0-9a-f]* \([0-9a-f]*\) .*/0x\1;\n/' >> secdat.c


rm -f *.tmp abc.c xyz.c x y z 



