#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "coltbase.h"

/*
 * Binary-expression:
    Unary-expression { binary-operator Unary-expression }
Binary-operator:
    cross, fetch, sift, sift-not, same, diff, join
    X, |, S, !S, ==, !=,
Unary-expression:
    subset-expression { row-expression/column-expression }
Column-expression:
    add, rename, select, define
Row-expression:
    if, sort, thru, cthru, agg, aggrow, part, share
Subset-expression:
    object-expression subset-operator object-expression <subset-agg>
    subset-operator(subset-expression-list) <subset-agg>
Subset-operator:
    isect, union, comp
    I, U, C
Subset-agg:
    range, search, agg, aggrow
Object-expression:
    object
    (binary-expression)
 */

int is_token(char *input, char * token)
{
	return strcmp(input, token) == 0;
}

char current_token[MAX_TOKEN];

char *consume_token(char *input)
{
	char *b = input;
	char *a = current_token;
	char quote_char = '\0';

	if(*b == '"' || *b == '\'') {
		quote_char = *b;
		while(*b && *b != quote_char)
			*a++ = *b++;
		*a = '\0';
	} else {
		while(*b != '\t' && *b != ' ' && *b != '\n')
			*a++ = *b++;
		*a = '\0';
	}

	return b;
}

colt_base *file_name(char *input)
{
	input = consume_token(input);
	return new colt_csv(current_token);
}

colt_base *colt_expression(char *input)
{
	return file_name(input);
}
