/*
 * coltyml.cpp
 *
 *  Created on: May 21, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltyml.h"
#include "coltnestedcells.h"

colt_yml::colt_yml(colt_base &in, char *fname):
	colt_operator(in),
	file_name(NULL),
	old(NULL)
{
	// TODO Auto-generated constructor stub

	if(fname) {
		file_name = new char[strlen(fname)+1];
		strcpy(file_name, fname);
	}

	if(file_name)
		out = new std::ofstream(file_name);
	else
		out = &cout;

	i_am = colt_class_yml;

}

colt_yml::~colt_yml() {
	// TODO Auto-generated destructor stub
	if(file_name)
		delete file_name;
}

int colt_yml::preprocess()
{
	COLT_TRACE("colt_out::preprocess()")
//	colt_operator::preprocess();
	return 1;
}

int colt_yml::process(int rec_num)
{
//	colt_nested_cells *current = nested_cells(rec_num);
////	int retval = current->nested_output(old, 0, out);
//	old = current;
//	num_cols();
	colt_nested_cells *current = nested_cells(rec_num);
	int retval = current->nested_output(old, 0, out);
	if(!old)
		old = new colt_nested_cells(current->cells, current->num_cols, current->headers, current->key, current->next, current->out);
	else {
		old->cells = current->cells;
		old->next = current->next;
	}

	old->index = current->index;

	return 1;
}
