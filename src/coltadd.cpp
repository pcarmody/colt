/*
 * coltadd.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltadd.h"

colt_add::colt_add(colt_base &in, char *col_name, int t, char *tstr):
	colt_cexpression(in, tstr),
	return_values(NULL)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_add;
	label = new char[strlen(col_name)+1];
	strcpy(label, col_name);
	type = t;
//	return_value = colt_datatype::new_object(type);
}

colt_add::~colt_add()
{
	// TODO Auto-generated destructor stub
	delete colt_add_out[num_cols()-1];
}

int colt_add::num_cols()
{
	COLT_TRACE("colt_add::num_cols()")
	return colt_cexpression::num_cols()+1;
}

char **colt_add::fields(int rec_num)
{
	COLT_TRACE("**colt_add::fields(int rec_num)")
	char **rec = colt_cexpression::fields(rec_num);

	if(!rec)
		return NULL;

	int cols = num_cols();
	int new_col = cols-1;
	for(int i=0; i<cols-1; i++)
		colt_add_out[i] = rec[i];

	if(!return_values[rec_num])
		cells(rec_num);
	else
		colt_add_cell[new_col]->set_value(return_values[rec_num]);

	if(!colt_add_out[new_col])
		colt_add_out[new_col] = new char[COLT_MAX_STRING_SIZE];

	colt_add_cell[new_col]->format(colt_add_out[new_col]);

	return colt_add_out;
}

colt_datatype **colt_add::cells(int rec_num)
{
	COLT_TRACE("**colt_add::cells(int rec_num)")
	colt_datatype **rec = colt_cexpression::cells(rec_num);

	if(!rec)
		return NULL;

	int cols = num_cols();

	if(return_values[rec_num]) {
		for(int i=0; i<cols-1; i++)
			colt_add_cell[i] = rec[i];
		colt_add_cell[cols-1]->set_value(return_values[rec_num]);

		return colt_add_cell;
	}

	return_values[rec_num] = colt_add_cell[cols-1]->make_space();

	void *cell_values[COLT_MAX_NUM_COLS];
	for(int i=0; i<cols-1; i++)
			cell_values[i] = rec[i]->get_value();

	if(type == COLT_DT_SOURCE) {
		colt_parser parse(code_string);

		colt_base *exp_object = parse.parse(1);

		exp_object->process_all();

		coltthru *thru = (coltthru *) exp_object->get_destination();

		colt_datatype *cell = colt_add_cell[cols-1];
		if(thru->is_a(colt_class_cthru))
			cell->set_type(COLT_DT_CTHRU);
		else if(thru->is_a(colt_class_sort))
			cell->set_type(COLT_DT_SORT);
		else if(thru->is_a(colt_class_thru))
			cell->set_type(COLT_DT_THRU);
		else if(thru->is_a(colt_class_range))
			cell->set_type(COLT_DT_RANGE);
		else if(thru->is_a(colt_class_thru))
			cell->set_type(COLT_DT_BITMAP);
		else {
			perror("Add column must add a thru object.\n");
			exit(1);
		}

		cell->set_value(thru);
	} else {
		_trace.start() << "call function.\n";
		cell_values[cols-1] = return_values[rec_num];
		if(function_ptr)
			(*function_ptr)((void **) cell_values);

		for(int i=0; i<cols-1; i++)
			colt_add_cell[i] = rec[i];
		colt_add_cell[cols-1]->set_value(return_values[rec_num]);

		if(!function_ptr) {
			delete return_values[rec_num];
			return_values[rec_num] = NULL;
		}
	}

	return colt_add_cell;
}

char *colt_add::col_header(int n)
{
	COLT_TRACE("*colt_add::col_header(int n)")
	if(n < colt_cexpression::num_cols())
		return colt_cexpression::col_header(n);

	return label;
}

char **colt_add::col_headers()
{
	COLT_TRACE("*colt_add::col_headers()")
	char **heads = colt_cexpression::col_headers();

	int cols = colt_cexpression::num_cols();
	for(int i=0; i<cols; i++)
		headers[i] = heads[i];
	headers[cols] = label;
	_trace.start() << "label = " << label << "\n";
	return headers;
}

int colt_add::preprocess()
{
	COLT_TRACE("colt_add::preprocess()")

	char *headers[100];
	int i;
	int cols = colt_cexpression::num_cols();
	char **heads = col_headers();
	for(i=0; i<cols; i++)
		headers[i] = heads[i];
	headers[i] = label;

	int numlines = num_lines();
	int sizeof_return_values = sizeof(colt_datatype *) * numlines;
	return_values = (void **) malloc(sizeof_return_values);

	for(i=0; i<numlines; i++)
		return_values[i] = NULL;

	colt_add_cell[cols] = new_datatype(type);
	colt_add_out[cols] = new char[COLT_MAX_STRING_SIZE];

	if(type == COLT_DT_SOURCE)
		return colt_operator::preprocess();

	return colt_cexpression::preprocess();
}

