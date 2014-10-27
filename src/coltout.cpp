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
		colt_operator(in)
{
	i_am = colt_class_out;
	if(col) {
		column_sep_char = new char[strlen(col)+1];
		strncpy(column_sep_char, col,1);
		column_sep_char[1] = '\0';
	}
	if(eol) {
		end_of_line_sep_char = new char[strlen(eol)+1];
		strncpy(end_of_line_sep_char, eol,1);
		end_of_line_sep_char[1] = '\0';
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

colt_base *colt_out::copy(colt_base *operand)
{
	return new colt_out(*operand, column_sep_char, end_of_line_sep_char, quote_char);
}

void colt_out::fill_sequential()
{
	char out_string[1000];
	out_string[0] = '\0';

	int k = 0;
//	for(colt_counter *j=operand->get_col_counter(); !j->done(); j->next()) {
	for(int j=0; j<num_cols(); j++) {
		if(k++ > 0)
			strcat(out_string, column_sep_char);
		strcat( out_string, extract_str( operand->col_header( j ) ) );
	}
	strcat(out_string, end_of_line_sep_char);
	cout << out_string;

//	for(colt_counter *i=operand->get_row_counter(); !i->done(); i->next()) {
	for(int i=0; i<num_lines(); i++) {
		int k = 0;
		out_string[0] = '\0';
		for(int j=0; j<num_cols(); j++) {
//		for(colt_counter *j=operand->get_col_counter(); !j->done(); j->next()) {
			if(k++ > 0)
				strcat(out_string, column_sep_char);
			strcat(out_string, extract_str(operand->field_val(i, j)));
		}
		strcat(out_string, end_of_line_sep_char);
		cout << out_string;
	}
}

int colt_out::preprocess()
{
	colt_operator::preprocess();
	string out_string;
	out_string[0] = '\0';

	int k = 0;

	for(int j=0; j<num_cols(); j++) {
		if(k++ > 0)
			out_string.append(column_sep_char);
		out_string.append(col_header(j));
	}

	out_string.append(end_of_line_sep_char);
	cout << out_string;

	return 1;
}

int colt_out::process(int i)
{
	char **f = fields(i);
	if(f == NULL)
		return 0;

	char out_string[500];// = (char *) malloc(500);
	out_string[0] = '\0';

	for(int j=0; j<num_cols(); j++) {
		if(j > 0)
			strcat(out_string, column_sep_char);

		char *x = extract_str(f[j]);
		int new_len = strlen(out_string)+strlen(x)+2;
		if(new_len > 500)
			break; //out_string = (char *) realloc(out_string, new_len);
		strcat(out_string, x );
	}
	strcat(out_string, end_of_line_sep_char);
	cout << out_string;
//	free(out_string);

	return 1;
}
