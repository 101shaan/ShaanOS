C_SOURCES_ALL = $(wildcard kernel/*/*.c)
# Exclude deprecated duplicates implemented elsewhere
C_SOURCES = $(filter-out kernel/driver/ATA.c kernel/driver/dadio.c,$(C_SOURCES_ALL))
ASM_SOURCES = $(wildcard kernel/*/*.asm)
C_OBJECTS = ${C_SOURCES:.c=.o}
ASM_OBJECTS = ${ASM_SOURCES:.asm=.o}

# Toolchain (override via environment if desired)
CC = i686-elf-gcc
LD = i686-elf-ld
AS = nasm
OBJCOPY = objcopy

CFLAGS ?= -Ikernel/include -ffreestanding -Wall -Werror -g
# If building with a native compiler fallback, try force 32-bit codegen
CFLAGS += -m32
ASFLAGS ?= -f elf32

.PHONY : all assemble run clean qemu-serial smoke

all: run

run : assemble
	qemu-system-i386 -drive format=raw,file=disk.img  -monitor stdio

qemu-serial: assemble
	qemu-system-i386 -nographic -serial stdio -monitor none -drive format=raw,file=disk.img

smoke: assemble
	@echo "[smoke] (placeholder)"

debug: assemble
	qemu-system-i386 -s -hda disk.img &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel/kernel.elf" -ex "b kmain" -ex "continue"

assemble: disk.img kernel.bin stage1.bin stage2.bin 
	dd if=stage1.bin of=disk.img bs=1 count=3 seek=0 skip=0 conv=notrunc
	dd if=stage1.bin of=disk.img bs=1 count=451 seek=62 skip=62 conv=notrunc
	mcopy -i disk.img stage2.bin kernel.bin ::  -D o


kernel.bin : kernel/kernel.elf  
	$(OBJCOPY) -O binary $^ $@
	chmod -x $@

#You can use the --print-map option to look at what the linker does

# Explicitly list expected objects to avoid stale/unknown files
OBJ_LIST = \
    kernel/driver/ata_driver.o \
    kernel/driver/keyboard.o \
    kernel/driver/timer.o \
    kernel/driver/cursor.o \
    kernel/hal/gdt.o \
    kernel/hal/inthandling.o \
    kernel/hal/interruptstubs.o \
    kernel/hal/x86.o \
    kernel/kernel/kernel.o \
    kernel/kernel/kshell.o \
    kernel/kernel/entry.o \
    kernel/kernel/stack.o \
    kernel/mem/phymem.o \
    kernel/mem/virtmem.o \
    kernel/driver/dadio_driver.o \
    kernel/driver/serial.o

kernel/kernel.elf : $(OBJ_LIST)
	$(LD) $^ -T kernel/linker.ld -e kmain -o $@ 
	chmod -x $@

%.o : %.c
	$(CC) $(CFLAGS) $< -c -o $@ 
%.o : %.asm
	$(AS) $< -o $@ $(ASFLAGS)

stage1.bin : boot/stage1/stage1.asm
	nasm $^ -f bin -o $@
stage2.bin: boot/stage2/stage2.asm
	nasm $^ -f bin -o $@
	
disk.img: 
	truncate $@ -s 1M
	mkfs.vfat -F12 -S512 -s1 $@
	
clean :
	rm -f $(OBJ_LIST) $(C_OBJECTS) $(ASM_OBJECTS) kernel/kernel.elf *.bin disk.img
