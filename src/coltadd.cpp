/*
 * coltadd.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltadd.h"


//colt_add::colt_add(colt_base &in):
//	colt_operator(in),
//	template_str(NULL)
//{
//// TODO Auto-generated constructor stub
//	i_am = colt_class_add;
//}

colt_add::colt_add(colt_base &in, char *col_name, int t, char *tstr):
	colt_cexpression(in, tstr)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_add;
	label = new char[strlen(col_name)+1];
	strcpy(label, col_name);
	type = t;
}

colt_add::~colt_add()
{
	// TODO Auto-generated destructor stub
}

int colt_add::num_cols()
{
	return colt_operator::num_cols()+1;
}

char **colt_add::fields(int rec_num)
{
	char **rec = colt_operator::fields(rec_num);

	if(!rec)
		return NULL;

	int i;

	for(i=0; i<colt_operator::num_cols(); i++) {
		colt_add_out[i] = rec[i];
	}

	int tmp = (*function_ptr)(rec);
//	colt_integer tmp1(tmp);
//	colt_add_out[i] = tmp.format();

	return colt_add_out;
}

colt_datatype **colt_add::cells(int rec_num)
{
	colt_datatype **rec = colt_operator::cells(rec_num);

	if(!rec)
		return NULL;

	int i;
	for(i=0; i<colt_operator::num_cols(); i++) {
		colt_add_cell[i] = rec[i];
	}

//	colt_add_cell[i] = (*function_ptr)(rec);

	return colt_add_cell;
}

char *colt_add::col_header(int n)
{
	if(n < colt_operator::num_cols())
		return colt_operator::col_header(n);

	return label;
}

int colt_add::preprocess()
{
	int retval = colt_operator::preprocess();

	char *headers[100];
	int i;
	for(i=0; i<colt_operator::num_cols(); i++)
		headers[i] = col_header(i);

	headers[i] = label;

	return retval;
}

