/*
 * coltout_vertical.cpp
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "colt_headers.h"
#include "colt_to_json.h"
#include <iostream>
#include <sstream>

using namespace std;

colt_to_json::colt_to_json(colt_base &in, char* key):
	colt_out(in, "</td><td>", "</td></tr>\n<tr><td>", ""),
	key_col_index(0),
	key_col(NULL)
{
	i_am = colt_class_to_json;
	if(key) {
		key_col = new char[strlen(key)+1];
		strcpy(key_col, key);
	}
}

colt_to_json::~colt_to_json() {
	// TODO Auto-generated destructor stub
}

colt_base *colt_to_json::copy(colt_base *op)
{
	COLT_TRACE("*colt_to_json::copy(colt_base *op)")
	return new colt_to_json(*op, key_col);
}

int colt_to_json::preprocess()
{
	COLT_TRACE("colt_to_json::preprocess()")
//	colt_operator::preprocess();

	key_col_index = -1;

	if(!key_col) {
		std::cout << "[";
		return 1;
	}


	int len = strlen(key_col);

	for(int j=0; j<num_cols(); j++) {
		if(strncmp(col_header(j), key_col, len) == 0) {
			key_col_index = j;
			break;
		}
	}

	std::cout << "{ ";

	return 1;
}

int colt_to_json::process(int rec_num)
{
	COLT_TRACE("colt_to_json::process(int i)")

	colt_datatype **cell_objs = cells(rec_num);
	char tmp_str[COLT_MAX_STRING_SIZE];

	if(key_col_index >= 0) {
		cell_objs[key_col_index]->format(tmp_str);
		std::cout << '"' << tmp_str << "\": ";
	}

	if(!gen_headers) {
		gen_headers = 1;
	} else
		std::cout << ",";

	std::cout << "{";
	int cols = num_cols();

	for(int j=0; j<cols; j++) {
		if(j != key_col_index) {
			cell_objs[j]->format(tmp_str);
			if(cell_objs[j]->type == COLT_DT_INTEGER)
				std::cout << "\"" << extract_str(col_header(j)) << "\": " << tmp_str ;
			else
				std::cout << "\"" << extract_str(col_header(j)) << "\": \"" << tmp_str << "\"";
			if(j<cols-1)
				std::cout << ",";
		}
	}
	cout << "}";

	return 1;
}

void colt_to_json::postprocess()
{
	COLT_TRACE("colt_to_json::postprocess()")
	if(key_col_index >= 0)
		std::cout << " }";
	else
		std::cout << " ]";
}
