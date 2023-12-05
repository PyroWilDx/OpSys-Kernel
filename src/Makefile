KERNEL = kernel.bin

SRCS = $(wildcard *.S *.c)

OBJS = $(strip crt0.o $(filter-out crt0.o,$(notdir $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SRCS))))))

CC = gcc
AS = gcc
LD = ld
OBJCOPY = objcopy
QEMU = /usr/bin/qemu-system-i386

CFLAGS = -m32 -Wall -Werror -g -gdwarf -std=c99 -nostdinc -fno-stack-protector -I. -fno-pie
ASFLAGS = -m32 -DASSEMBLER -g -gdwarf -I.
LDFLAGS = -melf_i386
QEMUOPTS = -cpu pentium2 -rtc base=localtime -m 64M -gdb tcp::1234 -S -kernel kernel.bin
QEMUOPTSRUN = -cpu pentium2 -rtc base=localtime -m 64M -kernel kernel.bin

.PHONY: all
all: clean kernel.bin

task_dump_screen.o: empty.o task_dump_screen.txt
	$(OBJCOPY) empty.o --add-section=.task_dump_screen=task_dump_screen.txt --set-section-flags=.task_dump_screen=contents,alloc,load,data task_dump_screen.o

kernel.bin: kernel.lds $(OBJS) task_dump_screen.o
	$(LD) $(LDFLAGS) -e entry -Tkernel.lds $(OBJS) task_dump_screen.o -o $@

clean:
	$(RM) $(OBJS) $(KERNEL) task_dump_screen.o

debug: $(KERNEL)
	$(QEMU) $(QEMUOPTS)

curses: $(KERNEL)
	$(QEMU) $(QEMUOPTS) -display curses

run: $(KERNEL)
	$(QEMU) $(QEMUOPTSRUN)

