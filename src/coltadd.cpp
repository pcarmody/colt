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
	return_value(NULL)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_add;
	label = new char[strlen(col_name)+1];
	strcpy(label, col_name);
	type = t;
	return_value = colt_datatype::new_object(type);
}

colt_add::~colt_add()
{
	// TODO Auto-generated destructor stub
}

int colt_add::num_cols()
{
	COLT_TRACE("colt_add::num_cols()")
	return colt_operator::num_cols()+1;
}

char **colt_add::fields(int rec_num)
{
	COLT_TRACE("**colt_add::fields(int rec_num)")
	char **rec = colt_operator::fields(rec_num);

	if(!rec)
		return NULL;

	int cols = num_cols();
	memcpy(colt_add_out, rec, sizeof(rec[0])*cols);
//	colt_add_out[cols] = return_value->format();

	int tmp = (*function_ptr)(colt_add_out);

	return_value->set_buffer(colt_add_out[cols]);

	return colt_add_out;
}

colt_datatype **colt_add::cells(int rec_num)
{
	COLT_TRACE("**colt_add::cells(int rec_num)")
	colt_datatype **rec = colt_operator::cells(rec_num);
	void *cell_values[COLT_MAX_NUM_COLS];

	if(!rec)
		return NULL;

	int cols = num_cols();
//	memcpy(colt_add_out, sizeof(rec[0])*cols, rec);
	for(int i=0; i<num_cols(); i++)
		cell_values[i] = rec[i]->value_ptr();
	cell_values[cols] = return_value->value_ptr();

	int tmp = (*function_ptr)((char **) cell_values);

	return colt_add_cell;
}

char *colt_add::col_header(int n)
{
	COLT_TRACE("*colt_add::col_header(int n)")
	if(n < colt_operator::num_cols())
		return colt_operator::col_header(n);

	return label;
}

int colt_add::preprocess()
{
	COLT_TRACE("colt_add::preprocess()")
	int retval = colt_operator::preprocess();

	char *headers[100];
	int i;
	for(i=0; i<colt_operator::num_cols(); i++)
		headers[i] = col_header(i);

	headers[i] = label;

	return retval;
}

