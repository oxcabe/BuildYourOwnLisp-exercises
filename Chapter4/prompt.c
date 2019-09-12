#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

char *readline(char *prompt) {
	fputs (prompt, stdout);
	fgets (buffer, 2048, stdin);
	return strdup (buffer);
}

void add_history(char *unused) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char **argv) {
	// REPL: Read, Evaluate, Print, Loop
	puts ("Welcome to REPL v0.0.1 (alpha)");
	puts ("Press Ctrl+c to Exit");
	
	while (1) {
		char *input = readline ("repl> ");
		add_history (input);
		printf ("%s\n", input);
		free (input);
	}
	return 0;
}
