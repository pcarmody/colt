/*
 * colthtml.cpp
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */
#include <stdlib.h>
#include <string.h>
#include "colt_headers.h"
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

int colt_html::headers_out()
{
	COLT_TRACE("colt_out::headers_out()")

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

int colt_html::preprocess()
{
	COLT_TRACE("colt_html::preprocess()")
	colt_operator::preprocess();
//	cout << "<table>\n<thead>\n<tr><th>";
//
//	char out_string[1000];
//	out_string[0] = '\0';
//
//	int k = 0;
//	for(int j=0; j<operand->num_cols(); j++) {
//		if(k++ > 0)
//			strcat(out_string, "</th><th>");
//		strcat(out_string, extract_str(operand->col_header(j)));
//	}
//	strcat(out_string, "</th></tr>\n");
//	cout << out_string;
//	cout << "</thead>\n<tbody>\n<tr><td>";

	return 1;
}

int colt_html::process(int rec_num)
{
	COLT_TRACE("colt_html::process(int rec_num)")
	cout << "<tr><td>";
	colt_out::process(rec_num);
}

void colt_html::postprocess()
{
	COLT_TRACE("colt_html::postprocess()")
	cout << "</tbody>\n</table>";
}
