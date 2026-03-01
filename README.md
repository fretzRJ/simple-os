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
sudo apt install build-essential grub-pc-bin grub-common xorriso mtools qemu-system-x86
```

### Fedora
```bash
sudo dnf install gcc binutils grub2-tools xorriso mtools qemu-system-x86
```

### Arch Linux
```bash
sudo pacman -S base-devel grub xorriso mtools qemu-system-x86
```

## Build
```bash
make
```

Produces: `build/simple-os.iso`

## If you get `Error 1` from `grub-mkrescue`
This usually means one of the GRUB ISO dependencies is missing.

1. Install required packages (example for Ubuntu/Debian):
   ```bash
   sudo apt install grub-pc-bin grub-common xorriso mtools
   ```
2. Rebuild:
   ```bash
   make clean && make
   ```
3. If it still fails, run this command directly and share the full output:
   ```bash
   grub-mkrescue -o build/simple-os.iso build/iso
   ```

## Run
```bash
make run
```

## Notes
- The text editor stores content only in RAM during runtime.
- This is an educational OS kernel, not a full production desktop OS.
