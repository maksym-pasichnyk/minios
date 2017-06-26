typedef unsigned int size_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned char *va_list;

#define NULL 0
#define INT_MAX 32767
#define va_start(list, param) (list = (((va_list)&param) + sizeof(param)))
#define va_end(list) (list = 0)
#define va_arg(list, type) (*(type *)((list += sizeof(type)) - sizeof(type)))

#define unused(value) ((void)value)

size_t strlen(const char *str);

void clear();

bool kbhit();

char getc();
char putc();

char getchar();
char putchar(char ch);
int printf(const char *format, ...);

void *memset(void *ptr, int value, size_t num);
void *memcpy(void *destination, const void *source, size_t num);
