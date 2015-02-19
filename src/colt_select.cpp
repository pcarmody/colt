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

colt_select::colt_select(colt_base &in, char **l, int c, int nom):
	colt_operator(in),
	no_match(nom),
	list(NULL),
	selected_headers(NULL)
{
	i_am = colt_class_select;
	headers = (char **) malloc(sizeof(char*) * c);
	count = c;

	for(int i=0; i<count; i++) {
		headers[i] = (char *) malloc(strlen(l[i]) + 2);
//		strcpy(headers[i], l[i]);
		sprintf(headers[i], "^%s$", l[i]);
	}
}

colt_select::~colt_select()
{
	// TODO Auto-generated destructor stub
	//free(index_list);
	free(selected_headers);
	delete list;
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
	return selected_headers[n];// colt_operator::col_header(list[n]);
}

char **colt_select::col_headers()
{
	COLT_TRACE("**colt_select::col_headers()")

	return selected_headers; // need to fix
}

int colt_select::preprocess()
{
	COLT_TRACE("colt_select::preprocess()")

	int cols = colt_operator::num_cols();
	int num = count;
	count = 0;

	if(list)
		delete list;
	list = new int[COLT_MAX_NUM_COLS];
	selected_headers = (char **) malloc(sizeof(char *) * cols);
	char **heads = operand->col_headers();

	for(int i=0; i<num; i++) {
		for(int j=0; j<cols; j++) {
			char *head = heads[j];
			if((no_match && !match(head, headers[i]) )
			|| (!no_match && match(head, headers[i]) ) ) {
				selected_headers[count] = head;
				list[count++] = j;
			}
		}
	}

	int retval = colt_operator::preprocess();

	return retval;
}

int colt_select::process(int rec_num)
{
	COLT_TRACE("colt_select::process(int rec_num)")

//	if(!list) {
//
//		int cols = colt_operator::num_cols();
//		int num = count;
//		list = new int[COLT_MAX_NUM_COLS];
//		selected_headers = (char **) malloc(sizeof(char *) * cols);
//		count = 0;
//
//		for(int j=0; j<cols; j++) {
//			char *head = operand->col_header( j );
//			for(int i=0; i<num; i++) {
//				if((no_match && !match(head, headers[i]) )
//				|| (!no_match && match(head, headers[i]) ) ) {
//	//			if(strcmp(headers[i], head ) == 0 ) {
//					selected_headers[count] = head;
//					list[count++] = j;
//					break;
//				}
//			}
//		}
//	}

	return colt_operator::process(rec_num);
}
