/*
 * coltoutcbf.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltoutcbf.h"

colt_out_cbf::colt_out_cbf(colt_base &in, char *fname):
	colt_operator(in),
	file_name(NULL),
	fileptr(NULL)
{
	// TODO Auto-generated constructor stub
	if(fname) {
		file_name = new char[strlen(fname)+1];
		strcpy(file_name, fname);
	}
}

colt_out_cbf::~colt_out_cbf()
{
	// TODO Auto-generated destructor stub
	if(file_name)
		delete file_name;
}

int colt_out_cbf::preprocess()
{
	if(file_name)
		fileptr = fopen(file_name, "wb");
	else
		fileptr = stdout;

	int cols = num_cols();
	fwrite(&cols, sizeof(cols), 1, fileptr);

	colt_datatype **type = cells(0);
	for(int i=0; i<cols; i++) {
		char *header = col_header(i);
		fwrite(header, strlen(header)+1, 1, fileptr);
		char buffer[COLT_MAX_STRING_SIZE];
		int len = type[i]->gen_header(buffer);
		fwrite(buffer, len, 1, fileptr);
	}

	return colt_operator::preprocess();
}

int colt_out_cbf::process(int i)
{
	char buffer[COLT_MAX_STRING_SIZE];
	colt_datatype **objects = cells(i);
	for(int i=0; i<num_cols(); i++) {
		int len = objects[i]->generate(buffer);
		fwrite(buffer, len, 1, fileptr);
	}

	return colt_operator::process(i);
}

void colt_out_cbf::postprocess()
{
	if(file_name)
		fclose(fileptr);

	colt_operator::postprocess();
}
