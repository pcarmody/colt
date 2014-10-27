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
	key_col_index(0)
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
	return new colt_to_json(*op, key_col);
}

int colt_to_json::preprocess()
{
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

int colt_to_json::process(int i)
{
	char **f = fields(i);
	if(!f)
		return 0;
	if(key_col_index >= 0)
		std::cout << '"' << extract_str(f[key_col_index]) << "\": [";
	else
		std::cout << "[";

	for(int j=0; j<num_cols(); j++) {
		if(j != key_col_index) {
			std::cout << "\n\t\"" << extract_str(col_header(j)) << "\": \"" << extract_str(f[j]) << "\"";
			if(j<num_cols()-1)
				std::cout << ",";
		}
	}

	cout << "\n],\n";
	return 1;
}

void colt_to_json::postprocess()
{
	if(key_col_index >= 0)
		std::cout << " }";
	else
		std::cout << " ]";
}
