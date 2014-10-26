/*
 * colthtml.cpp
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */
#include <stdlib.h>
#include <string.h>
#include "coltoperator.h"
#include "coltout.h"
#include "colt_html.h"
#include <iostream>
#include <sstream>

using namespace std;

colt_html::colt_html(colt_base &in):
		colt_out(in, "</td><td>", "</td></tr>\n", "")
{
	i_am = colt_class_html;
}

colt_html::~colt_html() {
	// TODO Auto-generated destructor stub
}

colt_base *colt_html::copy(colt_base *op)
{
	return new colt_html(*op);
}

void colt_html::fill_sequential()
{
	cout << "<table>\n<thead>\n<tr><th>";

//	colt_out::fill_sequential();
	char out_string[1000];
	out_string[0] = '\0';

	int k = 0;
	for(int j=0; j<operand->num_cols(); j++) {
		if(k++ > 0)
			strcat(out_string, "</th><th>");
		strcat(out_string, extract_str(operand->col_header(j)));
	}
	strcat(out_string, "</th></tr>\n");
	cout << out_string;
	cout << "</thead>\n<tbody>\n";

	for(int i=0; i<operand->num_lines(); i++) {
		int k = 0;
		out_string[0] = '\0';
		//for(int j=operand->first_col(); j != operand->is_last_col(); j=operand->next_col()) {
		for(int j=0; j<operand->num_cols(); j++) {
			if(k++ > 0)
				strcat(out_string, "</td><td>");
			strcat(out_string, extract_str(operand->field_val(i, j)));
		}
//		strcat(out_string, end_of_line_sep_char);
		cout << "<tr><td>" << out_string << "</td></tr>\n";
	}
	cout << "<tbody>\n</table>\n";
}

int colt_html::preprocess()
{
	colt_operator::preprocess();
	cout << "<table>\n<thead>\n<tr><th>";

	char out_string[1000];
	out_string[0] = '\0';

	int k = 0;
	for(int j=0; j<operand->num_cols(); j++) {
		if(k++ > 0)
			strcat(out_string, "</th><th>");
		strcat(out_string, extract_str(operand->col_header(j)));
	}
	strcat(out_string, "</th></tr>\n");
	cout << out_string;
	cout << "</thead>\n<tbody>\n<tr><td>";

	return 1;
}

int colt_html::process(int rec_num)
{
	cout << "<tr><td>";
	colt_out::process(rec_num);
}

void colt_html::postprocess()
{
	cout << "</tbody>\n</table>";
}
