## Branching workflow

- main: always bootable, stable only
- development: integration of features; must build and boot before merging to main
- feature/* and kernel/*, drivers/*, boot/*: short-lived branches off development
- release/*: cut from main when tagging versions

## Rules

- Commit small, focused changes with clear messages
- Merge feature -> development only when it compiles/boots
- Merge development -> main only when smoke passes (qemu-serial ok)
- Tag releases on main (e.g., v0.0.1)

## Make targets

- make kernel/kernel.elf -j
- make run
- make qemu-serial (serial logs)
- make clean


