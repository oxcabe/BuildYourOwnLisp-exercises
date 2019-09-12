#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

// #define DEBUG_MODE 1

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

unsigned int get_ast_height(mpc_ast_t *t, unsigned int height) {
	if (t->children_num == 0) {
		return height;
	} else {
		int i;
		unsigned int i_child;
		unsigned int max = 0;
		for (i = 0; i < t->children_num; i++) {
			i_child = get_ast_height (t->children[i], height + 1);
			if (i_child > max) {
				max = i_child;
			}
		}
		return max;
	}
}

#ifdef DEBUG_MODE
void debug_print_ast(mpc_ast_t *t) {
	unsigned int tree_height = get_ast_height (t, 1);

	printf ("[ %s <%s> ] ", t->tag, t->contents);
	if (tree_height != 1) {
		int i;
		for (i = 0; i < t->children_num; i++) {
			debug_print_ast (t->children[i]);
		}
		printf ("\n");
	}
}
#endif

long evaluate_operator(long x, char *op, long y) {
	switch (op[0]) {
	case '+':
		return x + y;
	case '-':
		return x - y;
	case '*':
		return x * y;
	case '/':
		return x / y;
	case '%':
		return x % y;
	}
	return 0;
}

long evaluate_polish_notation(mpc_ast_t *t) {
	if (strstr (t->tag, "number")) {
		return (atoi (t->contents));
	}
	char *op = t->children[1]->contents;
	long x = evaluate_polish_notation (t->children[2]);

	int i = 3;
	while (strstr (t->children[i]->tag, "expr")) {
		x = evaluate_operator (x, op, evaluate_polish_notation (t->children[i]));
		++i;
	}
	return x;
}


int main(int argc, char **argv) {
	/* Polish notation parser */
	mpc_parser_t *Number		= mpc_new ("number");
	mpc_parser_t *Operator		= mpc_new ("operator");
	mpc_parser_t *Expression	= mpc_new ("expression");
	mpc_parser_t *Repl			= mpc_new ("repl");

	mpca_lang (MPCA_LANG_DEFAULT,
		"																									\
			number		: /-?[0-9]+(\\.[0-9]+)?/ ;															\
			operator	: '+' | '-' | '*' | '/' | '%' | \"add\" | \"sub\" | \"mul\" | \"div\" | \"mod\" ;	\
			expression	: <number> | '(' <operator> <expression>+ ')' ;										\
			repl		: /^/ <operator> <expression>+ /$/ ;										\
			",
			Number, Operator, Expression, Repl);
	// REPL: Read, Evaluate, Print, Loop
	puts ("Welcome to REPL v0.0.1 (alpha)");
	puts ("Press Ctrl+c to Exit");
	
	while (1) {
		mpc_result_t r;
		char *input = readline ("repl> ");
		add_history (input);
		if (mpc_parse ("<stdin>", input, Repl, &r)) {
#ifdef DEBUG_MODE
			// Shows abstract tree nodes for debugging purposes
			debug_print_ast (r.output);
#endif
			printf ("%ld\n", evaluate_polish_notation (r.output));
			mpc_ast_delete (r.output);
		} else {
			mpc_err_print (r.error);
			mpc_err_delete (r.error);
		}
		free (input);
	}
	mpc_cleanup (4, Number, Operator, Expression, Repl);
	return 0;
}
