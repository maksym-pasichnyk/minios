#include "stdlib.h"
#include "io.h"

enum : uint8_t { DISPLAY_MAX_COUNT = 1 };

struct Console {
  uint32_t bgcol;
  uint32_t fgcol;
  uint32_t x;
  uint32_t y;
};

struct Display {
  Terminal terminals[DISPLAY_MAX_COUNT];

  static void Set(uint8_t id) {
    terminals[id]->set(id);
  }
};

DISPLAY *textmode_init() {

}

typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;

void syscall() {
	asm volatile("add $0x2c, %esp");
	asm volatile("pusha");
	int eax = 0, ebx = 0, ecx = 0, edx = 0;
	asm volatile("movl %%eax, %0":"=m"(eax));
	asm volatile("movl %%ebx, %0":"=m"(ebx));
	asm volatile("movl %%ecx, %0":"=m"(ecx));
	asm volatile("movl %%edx, %0":"=m"(edx));

  switch (eax) {

  }

  asm volatile("popa");
  asm volatile("iret");
}

void set_int(int i, uint32_t addr){
  *(uint16_t*)(0x2000 + 8 * i + 0) = (uint16_t)(addr & 0x0000ffff);
}
void syscal_init() {
	set_int(0x80, (uint32_t)syscall);
}

extern "C" void kernel_main() {
  Display::Set(0);
}
