#include "terminal.h"
#include "io.h"

static uint16_t *buffer = (uint16_t*) 0xB8000;
static const size_t WIDTH = 80, HEIGHT = 25;

Terminal terminal[0];

Terminal::Terminal() : x(0), y(0) {}

enum {
  BACKSPACE = 8, TAB, ENTER = 13, SHIFT = 16, CTRL, ALT, PAUSE_BREAK, CAPSLOCK, ESCAPE = 27,
  PAGEUP = 33, PAGEDOWN, END, HOME, LEFTARROW, UPARROW, RIGHTARROW, DOWNARROW, INSERT = 45, DELETE,
  _0 = 48, _1, _2, _3, _4, _5, _6, _7, _8, _9,
  a = 65, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z = 90,
  NUMPAD0 = 97, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,
  MULTIPLY, ADD, SUBSTRACT = 109, DECIMALPOINT, DIVIDE, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
  NUMLOCK = 144, SCROLLLOCK, SEMICOLON = 186, EQUALSIGN = 187, COMMA, DASH, PERIOD, FORWARDSLASH,
  GRAVEACCENT, OPENBRACKET = 219, BACKSLASH, CLOSEBRACKET, SINGLEQUOTE
};

unsigned char scancodes[] = {
  0, 27, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, 8, 9, 113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 91, 93, 13, 0, 97, 115,
  100, 102, 103, 104, 106, 107, 108, 59, 39, 96, 0, 92, 122, 120, 99, 118, 98, 110, 109, 44, 46, 47, 0, 42, 0, 32, 0, 59, 60, 61, 62, 63,
  64, 65, 66, 67, 68, 133, 134
};

void Terminal::clear() {
  for (int i = 0; i < 2000; i++) {
    buffer[i] = 1824;
  }
  x = 0;
  y = 0;
}

char Terminal::put(char ch) {
  buffer[80 * y + x] = 1792 | ch;
  if (++x == WIDTH) {
    if (++y == HEIGHT) { y = 0; }
    x = 0;
  }
  return ch;
}

bool Terminal::kbhit() {
  return ((char)inb(0x60)) > 0;
}

char Terminal::get() {
  while (!kbhit());
  char ch = inb(0x60);
  outb(0x60, 0);
  return scancodes[ch];
}
