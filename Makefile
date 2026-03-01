CC ?= gcc
LD ?= ld
AS ?= as

CFLAGS := -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostartfiles -Wall -Wextra -Werror
LDFLAGS := -m elf_i386 -T linker.ld
GRUB_MKRESCUE ?= $(shell command -v grub-mkrescue 2>/dev/null || command -v grub2-mkrescue 2>/dev/null)
XORRISO ?= $(shell command -v xorriso 2>/dev/null)

SRC_C := $(wildcard src/*.c)
OBJ_C := $(patsubst src/%.c,build/%.o,$(SRC_C))
OBJ_S := build/boot.o
OBJS := $(OBJ_S) $(OBJ_C)

.DEFAULT_GOAL := all

all: build/simple-os.iso

build:
	mkdir -p build

build/boot.o: src/boot.s | build
	$(AS) --32 $< -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

check-tools:
ifeq ($(strip $(GRUB_MKRESCUE)),)
	$(error grub-mkrescue not found. Install packages first: Debian/Ubuntu => sudo apt install grub-pc-bin grub-common xorriso mtools)
endif
ifeq ($(strip $(XORRISO)),)
	$(error xorriso not found. Debian/Ubuntu => sudo apt install xorriso)
endif

build/simple-os.iso: build/kernel.bin check-tools
	mkdir -p build/iso/boot/grub
	cp build/kernel.bin build/iso/boot/kernel.bin
	cp iso/boot/grub/grub.cfg build/iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ build/iso || { \
		echo ""; \
		echo "ISO creation failed. Common fix on Ubuntu/Debian:"; \
		echo "  sudo apt install grub-pc-bin grub-common xorriso mtools"; \
		echo "Then rerun: make clean && make"; \
		exit 1; \
	}

run: build/simple-os.iso
	qemu-system-i386 -cdrom build/simple-os.iso

help:
	@echo "Common commands:"
	@echo "  make            - Build kernel and ISO"
	@echo "  make run        - Run ISO in QEMU"
	@echo "  make clean      - Remove build artifacts"
	@echo ""
	@echo "Dependencies (Debian/Ubuntu):"
	@echo "  sudo apt install build-essential grub-pc-bin grub-common xorriso mtools qemu-system-x86"

clean:
	rm -rf build

.PHONY: all run clean check-tools help
