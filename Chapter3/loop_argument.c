#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc == 2) {
		unsigned int i, arg = atoi (argv[1]);
		for (i = 0; i < arg; ++i) {
			puts ("Hello World!");
		}
	}
	return 0;
}
