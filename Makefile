# Gather all C++ source files
CPP_SOURCES = $(wildcard kernel/*.cpp drivers/vga/*.cpp drivers/keyboard/*.cpp drivers/ports/*.cpp libc/stdlib/stdlib.cpp kernel/cpu/gdt/*.cpp kernel/cpu/interrupts/*.cpp kernel/memory/*.cpp)
HEADERS = $(wildcard kernel/*.h drivers/vga/*.h drivers/keyboard/*.h drivers/ports/*.h libc/stdlib/stdlib.h kernel/cpu/gdt/*.h kernel/cpu/interrupts/*.cpp kernel/memory/*.h)

# Replace .cpp with .o
OBJ = ${CPP_SOURCES:.cpp=.o}

# Compiler and linker
CPP = g++
LD = i686-elf-ld
NASM = nasm
QEMU = qemu-system-i386
GRUBMKRESCUE = grub-mkrescue

# Flags
CPPFLAGS = -g -m32 -ffreestanding -fno-pic -fno-builtin
LDFLAGS = -T linker.ld

# Main target
Ragnarok.bin: ${OBJ}
	$(NASM) -f elf32 boot/boot.s -o boot/boot.o
	$(NASM) -f elf32 kernel/cpu/gdt/gdt.s -o kernel/cpu/gdt/gdt_asm.o
	$(NASM) -f elf32 kernel/cpu/interrupts/stubs.s -o kernel/cpu/interrupts/stubs.o

	$(LD) $(LDFLAGS) -o $@ boot/boot.o kernel/cpu/gdt/gdt_asm.o kernel/cpu/interrupts/stubs.o $^
	mv Ragnarok.bin RagnarokOS/boot/Ragnarok.bin
	$(GRUBMKRESCUE) -o Ragnarok.iso RagnarokOS/

# Run target
run: Ragnarok.bin
	$(QEMU) Ragnarok.iso

# Clean target
clean:
	rm -f boot/*.o kernel/*.o Ragnarok Ragnarok.iso drivers/vga/*.o drivers/ports/*.o libc/stdlib/*.o RagnarokOS/boot/Ragnarok.bin
	rm -f kernel/cpu/gdt/*.o kernel/cpu/interrupts/*.o drivers/keyboard/*.o kernel/memory/*.o
