CC ?= gcc
LD ?= ld
AS ?= as

CFLAGS := -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostartfiles -Wall -Wextra -Werror
LDFLAGS := -m elf_i386 -T linker.ld

SRC_C := $(wildcard src/*.c)
OBJ_C := $(patsubst src/%.c,build/%.o,$(SRC_C))
OBJ_S := build/boot.o
OBJS := $(OBJ_S) $(OBJ_C)

all: build/simple-os.iso

build:
	mkdir -p build

build/boot.o: src/boot.s | build
	$(AS) --32 $< -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

build/simple-os.iso: build/kernel.bin
	mkdir -p build/iso/boot/grub
	cp build/kernel.bin build/iso/boot/kernel.bin
	cp iso/boot/grub/grub.cfg build/iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ build/iso >/dev/null 2>&1

run: build/simple-os.iso
	qemu-system-i386 -cdrom build/simple-os.iso

clean:
	rm -rf build

.PHONY: all run clean
