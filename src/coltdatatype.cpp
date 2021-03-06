/*
 * coltdatatype.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltdatatype.h"

colt_datatype *new_datatype(int type, char *h)
{
	return new colt_datatype(type, h);
}

int colt_datatype::format_thru(char *x)
{
	coltthru *thru = (coltthru *) value_type;

	return thru->to_string(x);
//	sprintf(x, "%d", thru->num_lines());

//	return strlen(x);
}

int colt_datatype::thru_size()
{
	return ((coltthru *) value_type)->format_size();
}

extern colt_base *colt_load_thru(char *file_name, int status);

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
//		else if(strcmp("range:", x) == 0)
//			return ((colt_range *) value_type)->from_string(x);
		else if(strncmp("bitmap:", x, 7) == 0)
			thru  =  new coltbitmap();
		else {
			colt_base *input = colt_load_thru(x);
			value_type = (value_type_t *) input;
			return x+strlen(x);
		}

		value_type = (value_type_t *) thru;
	}

	return thru->from_string(x);
	return NULL;
}


int colt_datatype::generate_thru(void *x)
{
	coltthru *thru = (coltthru *) value_type;

	return thru->generate(x);
}

char *colt_datatype::consume_thru(void *x)
{
	coltthru *thru=NULL;

	int *thru_type = (int *) x;

	if(*thru_type == colt_thru_version)
		thru = new coltthru();
	else if(*thru_type == colt_bitmap_version)
		thru = new coltbitmap();
//	else if(thru_type == colt_cthru_version)
//	else if(thru_type == colt_range_version)
//	else if(thru_type == colt_bitmap_version)
//	else if(thru_type == colt_keyspace_version)
//	else if(thru_type == colt_cbf_version)

	thru->consume(x);

	value_type = (value_type_t *) thru;

	return NULL;
}

int colt_datatype::compare_thru(colt_datatype &rite)
{
	coltthru *thru = (coltthru *) value_type;

	return 0;
}

