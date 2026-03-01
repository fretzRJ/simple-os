# SimpleOS (32-bit x86)

This repository contains a small but real bootable 32-bit operating system kernel for old x86 laptops/desktops.

## Features
- Boots with **GRUB** in 32-bit protected mode.
- Text-mode shell in VGA memory.
- Built-in commands:
  - `help`
  - `editor` (in-memory text editor)
  - `snake` (playable snake game)
  - `clear`
  - `about`

## Build requirements
Install a 32-bit capable compiler/linker plus GRUB ISO tools.

### Debian/Ubuntu
```bash
sudo apt update
sudo apt install build-essential grub-pc-bin xorriso qemu-system-x86
```

### Fedora
```bash
sudo dnf install gcc binutils grub2-tools xorriso qemu-system-x86
```

### Arch Linux
```bash
sudo pacman -S base-devel grub xorriso qemu-system-x86
```

## Build
```bash
make
```

Produces: `build/simple-os.iso`

If `make` reports `grub-mkrescue` or `xorriso` missing, install the packages above and run `make` again.

## Run
```bash
make run
```

## Notes
- The text editor stores content only in RAM during runtime.
- This is an educational OS kernel, not a full production desktop OS.
