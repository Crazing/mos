
######################
# Makefile for Mos #
######################


# Entry point of Mos
# It must be as same as 'KernelEntryPointPhyAddr' in load.inc!!!
ENTRYPOINT	= 0x30400

# Programs, flags, etc.
ASM		= nasm
CC		= gcc
LD		= ld
ASMBFLAGS	= -I boot/include/
ASMKFLAGS	= -I include/ -f elf
CFLAGS		= -w -m32 -I include -c -fno-builtin -fno-stack-protector
LDFLAGS		= -m elf_i386 -s -Ttext $(ENTRYPOINT)

# This Program
MOSBOOT	= boot/boot.bin boot/loader.bin
MOSKERNEL	= kernel.bin
OBJS		= kernel/kernel.o kernel/start.o kernel/i8259.o kernel/global.o kernel/protect.o\
			lib/klib.o lib/klibc.o lib/string.o
# All Phony Targets
.PHONY : everything final clean realclean all

# Default starting position
everything : $(MOSBOOT) $(MOSKERNEL)

all : realclean everything

final : all clean

clean :
	rm -f $(OBJS)

realclean :
	rm -f $(OBJS) $(MOSBOOT) $(MOSKERNEL)

boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/load.inc boot/include/fat12hdr.inc boot/include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(MOSKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(MOSKERNEL) $(OBJS)

kernel/kernel.o : kernel/kernel.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/start.o: kernel/start.c include/type.h include/const.h include/protect.h include/proto.h include/string.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o: kernel/i8259.c include/type.h include/const.h include/protect.h include/proto.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/global.o: kernel/global.c include/type.h include/const.h include/protect.h include/proto.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/protect.o: kernel/protect.c include/type.h include/const.h include/protect.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

lib/klibc.o: lib/klib.c include/type.h include/const.h include/protect.h include/proto.h include/string.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

lib/klib.o : lib/klib.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

lib/string.o : lib/string.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<
