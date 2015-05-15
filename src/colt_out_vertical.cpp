/*
 * coltout_vertical.cpp
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */
#include <stdlib.h>
#include <string.h>
#include "colt_headers.h"
#include "colt_out_vertical.h"
#include <iostream>
#include <sstream>

using namespace std;

colt_out_vertical::colt_out_vertical(colt_base &in, char sp):
		colt_out(in, "</td><td>", "</td></tr>\n<tr><td>", ""),
		col_seperator(sp),
		column_length(0)

{
	i_am = colt_class_out_vertical;
}

colt_out_vertical::~colt_out_vertical() {
	// TODO Auto-generated destructor stub
}

colt_base *colt_out_vertical::copy(colt_base *op)
{
	COLT_TRACE("*colt_out_vertical::copy(colt_base *op)")
	return new colt_out_vertical(*op, col_seperator);
}

void colt_out_vertical::fill_sequential()
{
	COLT_TRACE("colt_out_vertical::fill_sequential()")
	char out_string[1000];
	out_string[0] = '\0';

	int column_length = 0;

	int k = 0;
	for(int j=0; j<operand->num_cols(); j++) {
		int len = strlen(extract_str(operand->col_header(j)));
		if(column_length < len)
			column_length = len;
	}

	for(int i=0; i<operand->num_lines(); i++) {
		for(int j=0; j<operand->num_cols(); j++) {
			char *header = extract_str(operand->col_header(j));
			int len = strlen(header);
			cout << j+1;
			if(j<10)
				cout << "  ";
			else if(j<100)
				cout << " ";
			for(int k=len; k<=column_length; k++) cout << " ";
			cout << header;
			cout << ' ' << col_seperator << ' ' << extract_str(operand->field_val(i, j)) << "\n";
		}
		cout << "\n";
	}
}


int colt_out_vertical::preprocess()
{
	COLT_TRACE("colt_out_vertical::preprocess()")
	char out_string[1000];
	out_string[0] = '\0';

	column_length = 0;

	int k = 0;
	char **headers = col_headers();

	for(int j=0; j<num_cols(); j++) {
		int len = strlen(headers[j]);
		if(column_length < len)
			column_length = len;
	}

	col_seperator = sep_char();

	return 1;
}

int colt_out_vertical::process(int i)
{
	COLT_TRACE("colt_out_vertical::process(int i)")
	char **f = fields(i);
	if(!f)
		return 0;
	int k = 1;

	char col_sep = col_seperator;
	if(!col_sep) col_sep = '\t';

	char **headers = col_headers();

	for(int j=0; j<num_cols(); j++) {
		char *header = headers[j];
		int len = strlen(header);
		cout << k++;
		if(j<9)
			cout << "   ";
		else if(j<99)
			cout << "  ";
		else if(j<999)
			cout << " ";
		for(int k=len; k<=column_length; k++) cout << " ";
		cout << header;
		cout << ' ' << col_sep << ' ' << extract_str(f[j]) << "\n";
	}
	cout << "\n";

	return 1;
}
