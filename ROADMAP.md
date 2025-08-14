## ShaanOS Roadmap

### Vision
Build a small, clean x86 hobby OS that is fun to hack on, teach systems concepts, and provide a playground for future UI and language tooling.

### Near-term (0.1.x)
- Shell UX
  - Single clear command (`fresh`) and consistent help per command
  - Command args parsing improvements (quoted strings, numbers)
- Diagnostics
  - `hexdump` for memory ranges
  - `memmap` to show E820 and bitmap summaries
- Filesystem
  - Read simple files via FAT12, print file contents in shell
- Memory
  - Simple kernel heap (bump allocator) for dynamic allocations
- Timer/Keyboard
  - Adjustable PIT (done), add `sleep <ms>` in shell

### Mid-term (0.2.x)
- Tasking
  - Cooperative tasks with a very small scheduler (round-robin)
  - Timer tick preemption stub; later enable preemption
- Virtual memory
  - APIs for map/unmap pages; utilities for page table inspection
- Drivers
  - Serial console as primary I/O option toggleable from shell
  - Keyboard layout table selection

### GUI Spike (experimental branch)
- VGA mode switch and a basic pixel framebuffer
- Minimal compositor-like loop to draw windows/rects
- Input routing from keyboard to focused view

### Tooling
- Headless smoke test: boot + serial banner + shell ready
- CI: build and run smoke in QEMU

### Stretch/Future
- Text editor prototype in shell (line editor), then framebuffer editor in GUI branch
- Language tooling integration when your language is ready

### Good-first issues
- Add `sleep <ms>` using PIT
- Implement `hexdump <addr> <len>`
- Implement `memmap` readout
- Add shell command history (up/down)


