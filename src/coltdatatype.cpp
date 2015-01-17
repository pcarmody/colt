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

	return thru->to_string(x);
//	sprintf(x, "%d", thru->num_lines());

//	return strlen(x);
}

char *colt_datatype::parse_thru(char *x)
{
	coltthru *thru = NULL;

	if(value_type)
		thru = ((coltthru *) value_type);
	else {
		if(strncmp("thru:", x, 5) == 0)
			thru  =  new coltthru();
//		else if(strcmp("sort:", x) == 0)
//			return ((colt_sort *) value_type)->from_string(x);
//		else if(strcmp("cthru:", x) == 0)
//			return ((colt_cthru *) value_type)->from_string(x);
		else if(strcmp("range:", x) == 0)
			return ((colt_range *) value_type)->from_string(x);
//		else if(strcmp("bitmap:", x) == 0)
//			return ((coltbitmap *) value_type)->from_string(x);


		value_type = (value_type_t *) thru;
	}

	return thru->from_string(x);
	return NULL;
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

