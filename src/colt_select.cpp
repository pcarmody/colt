/*
 * colt_select.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <iostream>
#include "colt_headers.h"

using namespace std;

//
//  colt_select
//

colt_select::colt_select(colt_base &in, char **l, int c):
	colt_operator(in)
{
	i_am = colt_class_select;
	list = (int *) malloc(sizeof(int) * c);
	headers = (char **) malloc(sizeof(char*) * c);
	count = c;

	for(int i=0; i<count; i++) {
		headers[i] = (char *) malloc(strlen(l[i]));
		strcpy(headers[i], l[i]);
	}
}

colt_base *colt_select::copy(colt_base *op)
{
	return new colt_select(*op, headers, count);
}

colt_select::~colt_select()
{
	// TODO Auto-generated destructor stub
	//free(index_list);
}

int colt_select::num_cols()
{
	COLT_TRACE("colt_select::num_cols()")
	return count;
}

char **colt_select::fields(int rec_num)
{
	COLT_TRACE("**colt_select::fields(int rec_num)")
	char **rec = colt_operator::fields(rec_num);

	if(!rec)
		return NULL;

	for(int i=0; i<count; i++) {
		colt_select_out[i] = rec[list[i]];
	}

	return colt_select_out;
}

colt_datatype **colt_select::cells(int rec_num)
{
	COLT_TRACE("**colt_select::cells(int rec_num)")
	colt_datatype **rec = colt_operator::cells(rec_num);

	if(!rec)
		return NULL;

	for(int i=0; i<count; i++) {
		colt_select_cells[i] = rec[list[i]];
	}

	return colt_select_cells;
}


char *colt_select::col_header(int n)
{
	COLT_TRACE("*colt_select::col_header(int n)")
	return colt_operator::col_header(list[n]);
}

int colt_select::preprocess()
{
	COLT_TRACE("colt_select::preprocess()")
	if(list)
		delete list;
	list = new int[count];

	int cols = colt_operator::num_cols();
	for(int j=0; j<cols; j++) {
		char *head = operand->col_header( j );
		for(int i=0; i<count; i++) {
			if(strcmp(headers[i], head ) == 0 ) {
				list[i] = j;
			}
		}
	}

	int retval = colt_operator::preprocess();

	return retval;
}
