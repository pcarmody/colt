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
	for(int i=0; i<cols-1; i++)
		colt_add_out[i] = rec[i];
//	memcpy(colt_add_out, rec, sizeof(rec[0])*cols);
	colt_add_out[cols] = (char *) return_values[rec_num];

	(*function_ptr)((void **) colt_add_out);

	return_values[rec_num] = (void *) colt_add_out[cols];

	return colt_add_out;
}

colt_datatype **colt_add::cells(int rec_num)
{
	COLT_TRACE("**colt_add::cells(int rec_num)")
	colt_datatype **rec = colt_cexpression::cells(rec_num);
	if(rec_num == -1)
		return rec;

	void *cell_values[COLT_MAX_NUM_COLS];

	if(!rec)
		return NULL;

	int cols = num_cols();
	for(int i=0; i<cols-1; i++)
		cell_values[i] = rec[i]->get_value();
	cell_values[cols] = return_values[rec_num];

//	if(cell_values[cols])
//		return colt_add_cell;

	_trace.start() << "call function.\n";
	int tmp = (*function_ptr)((void **) cell_values);

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
	return headers;
}

int colt_add::preprocess()
{
	COLT_TRACE("colt_add::preprocess()")
	int retval = colt_cexpression::preprocess();

	char *headers[100];
	int i;
	for(i=0; i<colt_cexpression::num_cols(); i++)
		headers[i] = col_header(i);
	headers[i] = label;

	int numlines = num_lines();
	int sizeof_return_values = sizeof(colt_datatype *) * numlines;
	return_values = (void **) malloc(sizeof_return_values);

	for(i=0; i<numlines; i++)
		return_values[i] = NULL;

	return retval;
}

