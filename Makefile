
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
ASMKFLAGS	= -I include/interrupt/ -I include/lib/ -I include/main/ -f elf
CFLAGS		= -w -m32 -I include -I include/interrupt -I include/lib -I include/main\
			  -I include/process -I include/tty -c -fno-builtin -fno-stack-protector
LDFLAGS		= -m elf_i386 -s -Ttext $(ENTRYPOINT)

# This Program
MOSBOOT		= boot/boot.bin boot/loader.bin
MOSKERNEL	= kernel.bin

MSOURCES	= src/main/kernel.asm src/interrupt/interrupt.asm src/interrupt/sys_call.asm\
			  src/lib/common.asm src/lib/string.asm 
CSOURCES	= src/interrupt/8259A.c src/interrupt/clock.c src/interrupt/interrupt.c\
			  src/interrupt/sys_call.c src/lib/common.c src/lib/string.c src/main/kernel.c\
			  src/process/process.c src/tty/console.c src/tty/keyboard.c src/tty/keymap.c\
			  src/tty/tty.c

MOBJS		= $(MSOURCES:.asm=_s.o)
COBJS		= $(CSOURCES:.c=.o)
OBJS		= $(MOBJS) $(COBJS)

# All Phony Targets
.PHONY : everything final image clean realclean all buildimg

# Default starting position
everything : $(MOSBOOT) $(MOSKERNEL)

all : realclean everything

final : all clean

image : final buildimg

clean :
	rm -f $(OBJS)

realclean :
	rm -f $(OBJS) $(MOSBOOT) $(MOSKERNEL)

# Write "boot.bin" & "loader.bin" into floppy image "TINIX.IMG"
# We assume that "TINIX.IMG" exists in current folder
buildimg :
	mount _dbgmos/mos.img /mnt/floppy -o loop
	cp -f boot/loader.bin /mnt/floppy/
	cp -f kernel.bin /mnt/floppy
	umount  /mnt/floppy

# include
-include $(CSOURCES:.c=.d)

#模式规则
%.d: %.c 
	set -e; rm -f $@;\
	$(CC) -MM $(CFLAGS) $< > $@.$$$$;\
	sed 's,\(.*\)\.o:,$*.o $@:,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

boot/boot.bin   : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/load.inc boot/include/fat12hdr.inc boot/include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(MOSKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(MOSKERNEL) $(OBJS)

$(MOBJS)	 : src/%_s.o: src/%.asm include/%.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

