/*
 * coltmetadata.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltmetadata.h"

colt_metadata::colt_metadata(colt_base &in):
	colt_operator(in)
{
	// TODO Auto-generated constructor stub

}

colt_metadata::~colt_metadata() {
	// TODO Auto-generated destructor stub
}

int	colt_metadata::num_cols()
{
	return operand->meta_num_cols();
}

char *colt_metadata::col_header(int n)
{
	return operand->meta_col_header(n);
}

char **colt_metadata::col_headers()
{
	return operand->meta_col_headers();
}

char **colt_metadata::fields(int rec_num)
{
	return operand->meta_fields(rec_num);
}

colt_datatype **colt_metadata::cells(int rec_num)
{
	return operand->meta_cells(rec_num);
}

int colt_metadata::process(int row)
{
	int meta_index = -1;

	while((meta_index = operand->get_meta_row(meta_index)) >= 0)
		out_object->process(meta_index);

	throw "exiting metadata";

	return 1;

}
