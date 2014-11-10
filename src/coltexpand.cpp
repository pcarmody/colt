/*
 * coltexpand.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltdtindex.h"
#include "coltexpand.h"

colt_expand::colt_expand(colt_base &in, char *cname, char *exp):
	colt_cross(in, NULL),
	expression(NULL),
	col_name(NULL),
	col_number(-1),
	dt_thru(NULL)
{
	// TODO Auto-generated constructor stub
	col_name = new char[strlen(cname)+1];
	strcpy(col_name, cname);
	expression = new char[strlen(exp)+1];
	strcpy(expression, cname);
}

colt_expand::~colt_expand() {
	// TODO Auto-generated destructor stub
	delete col_name;
}

int colt_expand::preprocess()
{
	for(int j=0; j<colt_operator::num_cols(); j++) {
		char *head = operand->col_header( j );
		if(strcmp(head, col_name ) == 0 ) {
			col_number = j;
			break;
		}
	}

	if(col_number < 0) {
		std::cerr << "Expand can't find column '" << col_name << "'\n";
		throw;
	}

	dt_thru = (colt_dt_index *) colt_cross::cells(0)[col_number];
	if(dt_thru->isa_container()) {
		std::cerr << "Expand expected column '" << col_name << "' to be a container.\n";
		throw;
	}

	right = dt_thru->get_thru();

	return colt_cross::preprocess();
}

int colt_expand::process(int rec_num)
{
	colt_each tmp(*right, (char *) expression);
	right = &tmp;
	return colt_cross::process(rec_num);
}
