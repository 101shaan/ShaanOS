# Shaan's Operating System

A custom x86 operating system kernel written in C and Assembly, featuring memory management, process scheduling, interrupt handling, and basic I/O. This project demonstrates low-level systems programming and OS design from scratch.

## Features

- **x86 Architecture Support:** Designed for 32-bit Intel x86 CPUs.
- **Memory Management:**
  - Physical memory manager (`phymem.c`): Bitmap-based allocation for up to 4GB RAM.
  - Virtual memory manager (`virtmem.c`): Paging, recursive mapping, and identity map removal.
- **Process & Interrupt Handling:**
  - Programmable Interrupt Controller (PIC) and IDT setup (`inthandling.c`).
  - Timer and keyboard interrupt support.
- **Basic I/O:**
  - VGA text mode output and color control (`dadio_driver.c`).
  - Keyboard input driver with scancode translation.
  - ATA PIO disk read support (`ata_driver.c`).
- **File System:**
  - FAT12 support for reading files from disk images.
- **Kernel Shell:**
  - Simple command-line shell (`kshell.c`) for user interaction and testing.
  - Commands: `help`, `fresh`, `timer`, `echo`, `uptime`, `mem`, `name`

## Directory Structure

```
OS/
├── Makefile
├── script.sh
├── boot/
│   ├── stage1/         # Bootloader stage 1 (assembly)
│   └── stage2/         # Bootloader stage 2 (assembly, GDT, paging)
├── kernel/
│   ├── driver/         # Device drivers (ATA, keyboard, timer, VGA)
│   ├── hal/            # Hardware abstraction layer (GDT, interrupts)
│   ├── include/        # Kernel headers
│   ├── kernel/         # Kernel core (entry, shell)
│   └── mem/            # Memory management (physical, virtual)
```

## Building & Running

### Prerequisites

- **Cross-compiler:** `i686-elf-gcc`, `i686-elf-ld`, `nasm`
- **Emulator:** `qemu-system-i386`
- **Utilities:** `mcopy`, `truncate`, `mkfs.vfat`

### Build Steps

1. **Build the kernel and bootloader:**
   ```sh
   make
   ```
2. **Run in QEMU:**
   ```sh
   make run
   ```

### Debugging

## Roadmap

See `ROADMAP.md` for planned features, priorities, and milestones.

- Start QEMU in debug mode and connect with GDB:
  ```sh
  make debug
  ```

## Key Components

- **Memory Management:**  
  - `phymem.c`, `virtmem.c`, `phymem.h`, `virtmem.h`
- **Drivers:**  
  - `ata_driver.c`, `dadio_driver.c`, `keyboard.c`, `timer.c`
- **HAL:**  
  - `hal_driver.c`, `inthandling.c`, `gdt.c`
- **Kernel:**  
  - `kernel.c`, `kshell.c`, `entry.asm`
- **Bootloader:**  
  - `stage1.asm`, `stage2.asm`, `GDT.asm`, `pagingsetup.asm`

## Usage

- On boot, the kernel initializes memory, sets up interrupts, and launches a simple shell.
- The shell supports basic commands for testing hardware and kernel features.

## Credits

- Inspired by [OSDev Wiki](https://wiki.osdev.org/) and [BrokenThorn OS Development Series](http://www.brokenthorn.com/Resources/OSDevIndex.html).

## License

This project is for educational purposes. See LICENSE for details.
