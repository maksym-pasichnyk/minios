#include "stdlib.h"

struct Terminal {
	int x, y;

	Terminal();

	void clear();

	bool kbhit();
	char put(char ch);
	char get();
};
