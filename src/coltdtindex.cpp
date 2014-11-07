/*
 * coltdtindex.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltdtindex.h"

colt_dt_index::colt_dt_index(char *file_name):
	colt_integer()
{
	// TODO Auto-generated constructor stub
	type = COLT_DT_INDEX;
	thru_file_name = new char[strlen(file_name)+1];
	strcpy(thru_file_name, file_name);
//	thru = colt_load_thru(thru_file_name);
}

colt_dt_index::~colt_dt_index() {
	// TODO Auto-generated destructor stub
	delete thru_file_name;
}

int colt_dt_index::gen_header(char *x)
{
	int len = colt_integer::gen_header(x);
	strcpy(x+len, thru_file_name);

	return len+strlen(thru_file_name)+1;
//	memcpy(x, &type, sizeof(type));
//
//	return sizeof(type);
}

coltthru *colt_dt_index::get_thru()
{
	if(!thru)
		thru = (coltthru *) colt_load_thru(thru_file_name);
	return thru;
}
