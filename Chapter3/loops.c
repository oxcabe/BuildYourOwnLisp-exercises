#include <stdio.h>

int main(int argc, char **argv) {
	int i;

	puts ("For loop iterations");
	for (i = 0; i < 5; ++i) {
		puts ("Hello World!");
	}
	puts ("While loop iterations");
	i = 0;
	while (i < 5) {
		puts ("Hello World!");
		++i;
	}
	return 0;
}
