# SimpleOS (32-bit x86)

This repository now contains a small but real bootable 32-bit operating system kernel for old x86 laptops/desktops.

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
On Debian/Ubuntu-like systems:

```bash
sudo apt install build-essential grub-pc-bin xorriso qemu-system-x86
```

## Build

```bash
make
```

Produces: `build/simple-os.iso`

## Run

```bash
make run
```

## Notes
- The text editor stores content only in RAM during runtime.
- This is an educational OS kernel, not a full production desktop OS.
