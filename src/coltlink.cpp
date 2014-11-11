/*
 * coltlink.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltlink.h"

colt_link::colt_link(colt_base &in, char *colname, int type, char *exp):
	colt_each(in, exp),
	datatype(type),
	column_name(NULL)
{
	// TODO Auto-generated constructor stub
	column_name = new char[strlen(colname)+1];
}

colt_link::~colt_link()
{
	// TODO Auto-generated destructor stub
	delete column_name;
}

