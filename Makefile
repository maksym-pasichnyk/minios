CC = i686-elf-g++
CCFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

$(shell mkdir -p output/o)

clean:
	@rm -rf output
	@mkdir -p output/o

build: kernel.bin
rebuild: clean build

run: build
	@qemu-system-x86_64 -kernel output/kernel.bin
	@#cp output/kernel.bin iso/boot/kernel.bin
	@#grub-mkimage -o pixelos.iso iso
	@#qemu-system-x86_64 -cdrom pixelos.iso -m 256M

all: clean run

kernel.bin:
	@i686-elf-as -o output/o/boot.o source/boot.s
	@#@nasm -o output/o/boot.o source/boot.asm -f elf
	@$(CC) $(CCFLAGS) -c -o output/o/kernel.o source/kernel.cpp
	@$(CC) $(CCFLAGS) -c -o output/o/stdlib.o source/stdlib.cpp
	@$(CC) $(CCFLAGS) -c -o output/o/pci.o source/pci.cpp
	@$(CC) $(CCFLAGS) -c -o output/o/terminal.o source/terminal.cpp
	@$(CC) -T linker.ld -o output/kernel.bin -ffreestanding -O2 -nostdlib		\
		output/o/boot.o																												\
		output/o/kernel.o																											\
		output/o/stdlib.o																											\
		output/o/pci.o																												\
		output/o/terminal.o																										\
	-lgcc
