/*
 * coltadd.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltadd.h"

colt_add::colt_add(colt_base &in, char *col_name, int t, char *tstr):
	colt_operator(in),
	label(NULL)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_add;
	if(col_name) {
		label = new char[strlen(col_name)+1];
		strcpy(label, col_name);
	}
	type = t;
	strcpy(value, tstr);
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
	if(label)
		return colt_operator::num_cols()+1;
	return colt_operator::num_cols();
}

int colt_add::compare(int a, int b, int c)
{
	COLT_TRACE("colt_add::compare(int a, int b, int c)")
	if(c < num_cols()-1  || !label)
		return colt_operator::compare(a,b,c);

	colt_datatype left(type);
	colt_datatype rite(type);

	int cols = num_cols();
	get_value(a);
	left.set_value(colt_add_cell[cols-1]->get_value());

	return left.compare(*colt_add_cell[cols-1]);
}

void colt_add::get_value(int rec_num)
{
	int cols = num_cols();
	if(colt_add_cell[cols-1])
		colt_add_cell[cols-1]->set_value(value);
}

char **colt_add::fields(int rec_num)
{
	COLT_TRACE("**colt_add::fields(int rec_num)")
	char **rec = colt_operator::fields(rec_num);

	if(!label)
		return rec;

	if(!rec)
		return NULL;

	get_value(rec_num);

	int cols = num_cols();
	int new_col = cols-1;
	int i;
	for(i=0; i<cols-1; i++)
		colt_add_out[i] = rec[i];

	colt_add_out[i] = value;

	return colt_add_out;
}

colt_datatype **colt_add::cells(int rec_num)
{
	COLT_TRACE("**colt_add::cells(int rec_num)")
	colt_datatype **rec = colt_operator::cells(rec_num);

	if(!label)
		return rec;

	if(!rec)
		return NULL;

	get_value(rec_num);

	int cols = num_cols();
	for(int i=0; i<cols-1; i++)
		colt_add_cell[i] = rec[i];

	if(colt_add_cell[cols-1]) {
//		colt_add_cell[cols-1]->set_value(value);

		return colt_add_cell;
	}

	colt_datatype *new_cell = new_datatype(type);//(colt_datatype *) colt_add_cell[cols-1]->make_space();
	colt_add_cell[cols-1] = new_cell;
	colt_add_cell[cols-1]->set_value(value);

	return colt_add_cell;
}

char *colt_add::col_header(int n)
{
	COLT_TRACE("*colt_add::col_header(int n)")
	if(n < colt_operator::num_cols())
		return colt_operator::col_header(n);

	return label;
}

char **colt_add::col_headers()
{
	COLT_TRACE("*colt_add::col_headers()")
	char **heads = colt_operator::col_headers();

	if(!label)
		return heads;

	int cols = colt_operator::num_cols();
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
	int cols = colt_operator::num_cols();
	char **heads = col_headers();
	for(i=0; i<cols; i++)
		headers[i] = heads[i];
	headers[i] = label;

	colt_add_cell[cols] = new_datatype(type);

	return colt_operator::preprocess();
}

