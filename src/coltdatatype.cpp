/*
 * coltdatatype.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltdatatype.h"

colt_datatype *new_datatype(int type)
{
	return new colt_datatype(type);
}

int colt_datatype::format_thru(char *x)
{
	coltthru *thru = (coltthru *) value_type;

	sprintf(x, "%d", thru->num_lines());

	return strlen(x);
}

int colt_datatype::generate_thru(char *x)
{
	coltthru *thru = (coltthru *) value_type;

	return 0;
}

char *colt_datatype::consume_thru(char *x)
{
	coltthru *thru = (coltthru *) value_type;

	return NULL;
}

int colt_datatype::compare_thru(colt_datatype &rite)
{
	coltthru *thru = (coltthru *) value_type;

	return 0;
}

