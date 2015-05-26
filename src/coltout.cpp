/*
 * coltoout.cpp
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "colt_headers.h"
#include "coltout.h"
#include <iostream>
#include <sstream>

using namespace std;

colt_out::colt_out(colt_base &in, char *col, char *eol, char *quote):
		colt_operator(in),
		quote_char(NULL),
		gen_headers(0)
{
	i_am = colt_class_out;
	if(col) {
		column_sep_char = new char[strlen(col)+1];
//		strncpy(column_sep_char, col,1);
		strcpy(column_sep_char, col);
//		column_sep_char[1] = '\0';
	}
	if(eol) {
		end_of_line_sep_char = new char[strlen(eol)+1];
		strcpy(end_of_line_sep_char, eol);
//		strncpy(end_of_line_sep_char, eol,1);
//		end_of_line_sep_char[1] = '\0';
	}
	if(quote) {
		quote_char = new char[strlen(quote)+1];
		strncpy(quote_char, quote,1);
		quote_char[1] = '\0';
	}
}

colt_out::~colt_out() {
	if(column_sep_char)
		delete column_sep_char;
	if(end_of_line_sep_char)
		delete end_of_line_sep_char;
	if(quote_char)
		delete quote_char;
}

int colt_out::headers_out()
{
	COLT_TRACE("colt_out::headers_out()")
	string out_string;
	out_string[0] = '\0';

	int k = 0;

	char **heads = col_headers();

	int cols = num_cols();
	for(int j=0; j<cols; j++) {
		if(k++ > 0)
			out_string.append(column_sep_char);
		out_string.append(heads[j]);
	}

	out_string.append(end_of_line_sep_char);
	cout << out_string;

	return 1;
}

int colt_out::preprocess()
{
	COLT_TRACE("colt_out::preprocess()")
//	colt_operator::preprocess();
	return 1;
}

int colt_out::process(int i)
{
	COLT_TRACE("colt_out::process(int i)");
//	static colt_nested_cells *old;
//	colt_nested_cells *current = nested_cells(i);
//	int retval = current->nested_output(old, 0, &cout);
//	if(!old)
//		old = new colt_nested_cells(current->cells, current->num_cols, current->headers, current->key, current->next, current->out);
//	else {
//		old->cells = current->cells;
//		old->next = current->next;
//		old->index = current->index;
//	}

	if(!gen_headers)
		gen_headers = headers_out();

	char **f = fields(i);
	if(f == NULL)
		return 0;

	char out_string[COLT_MAX_STRING_SIZE];// = (char *) malloc(500);
	out_string[0] = '\0';

	int cals = num_cols();

	for(int j=0; j<cals; j++) {
		if(j > 0)
			strcat(out_string, column_sep_char);

		char *x = extract_str(f[j]);
		int new_len = strlen(out_string)+strlen(x)+2;

		if(quote_char)
			new_len += 2;

		if(new_len > COLT_MAX_STRING_SIZE)
			break; //out_string = (char *) realloc(out_string, new_len);

		if(quote_char)
			sprintf(out_string,"%s%s%s%s", out_string, quote_char, x, quote_char);
		else
			strcat(out_string, x );
	}
	strcat(out_string, end_of_line_sep_char);
	fwrite(out_string, strlen(out_string), 1, stdout);
//	cout << out_string;
//	free(out_string);

	return 1;
}
