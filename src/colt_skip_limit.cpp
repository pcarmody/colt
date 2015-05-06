/*
 * colt_skip_limit.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "colt_headers.h"

using namespace std;

//
//  colt_skip_limit
//

colt_skip_limit::colt_skip_limit(colt_base &in, int limit, int skip):
	colt_operator(in),
	limit_to(limit),
	skip_by(skip),
	rec_counter(0)
{
	i_am = colt_class_skip_limit;
}

colt_skip_limit::~colt_skip_limit()
{
	// TODO Auto-generated destructor stub
	//free(index_list);
}

colt_base *colt_skip_limit::copy(colt_base *op)
{
	COLT_TRACE("*colt_skip_limit::copy(colt_base *op)")
	return new colt_skip_limit(*op, limit_to, skip_by);
}
void colt_skip_limit::fill_sequential(int start)
{
	COLT_TRACE("colt_skip_limit::fill_sequential(int start)")
//	for(int i=skip_by; i<limit_to+skip_by; i++)
////		index_list[index_count++] = i;
//		push_back(i);
}

int colt_skip_limit::num_lines()
{
	return limit_to;
}


int colt_skip_limit::preprocess()
{
	COLT_TRACE("colt_skip_limit::preprocess()")
	rec_counter = 0;
	return colt_operator::preprocess();
}

int colt_skip_limit::process(int rec_num)
{
	COLT_TRACE("colt_skip_limit::process(int rec_num)")
	rec_counter++;

	if(rec_counter < skip_by && rec_counter <= num_lines())
		return NULL;

	if(rec_counter >= skip_by+limit_to
	|| rec_counter > num_lines())
		return 1;
//		throw new colt_exception();

	return colt_operator::process(rec_num);
}
