#ifndef PANIC_H
#define PANIC_H

void panic(const char* msg);

#define ASSERT(cond) do { if (!(cond)) panic("ASSERT failed: " #cond); } while (0)

#endif

