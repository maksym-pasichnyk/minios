#include "terminal.h"
#include "stdlib.h"
#include "math.h"

extern Terminal *terminal;


size_t strlen(const char *str) {
	size_t length = 0;
	while (str[length]) { length++; }
	return length;
}

void clear() {
  terminal->clear();
}

bool kbhit() {
	return terminal->kbhit();
}

char getc() {
	return terminal->get();
}

char putc(char ch) {
	return terminal->put(ch);
}

char getchar() {
	return terminal->put(terminal->get());
}

char putchar(char ch) {
	terminal->put(ch);
  return ch;
}

void print_int(int value) {
  int count = 1, flag;
  if (flag = value < 0) {
    value = -value;
    count += 1;
  }
  count += log10(value);

  char res[count];
  res[count--] = 0;

  while (value != 0) {
    res[count--] = value % 10 + '0';
    value /= 10;
  }

  if (flag) {
    res[0] = '-';
  }

  printf(res);
}

static bool print(int value) {
	print_int(value);
	return true;
}

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == 0)
			return false;
	return true;
}

int printf(const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
					return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

void *memset(void *ptr, int value, size_t num) {
  for (size_t i = 0; i < num; i++) {
    ((uint8_t*)ptr)[i] = (uint8_t)value;
  }
  return ptr;
}

void *memcpy(void *destination, const void *source, size_t num) {
  for (size_t i = 0; i < num; i++) {
    ((char*)destination)[i] = ((char*)source)[i];
  }
  return destination;
}
