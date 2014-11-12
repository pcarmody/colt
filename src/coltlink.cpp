/*
 * coltlink.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltlink.h"

colt_link::colt_link(colt_base &in, char *colname, int type, char *fname, char *exp):
	colt_each(in, exp),
	datatype(type),
	filename(NULL),
	column_name(NULL)
{
	// TODO Auto-generated constructor stub
	column_name = new char[strlen(colname)+1];
	strcpy(filename, fname);
	filename = new char[strlen(fname)+1];
	strcpy(filename, fname);
}

colt_link::~colt_link()
{
	// TODO Auto-generated destructor stub
	delete column_name;
	delete filename;
}

