/*
 * colt_range_list.cpp
 *
 *  Created on: June 26, 2014
 *      Author: paulcarmody
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include "colt_headers.h"

colt_range_list::colt_range_list(colt_base &in):
	colt_operator(in),
	ranges(NULL),
	range_count(0)
{
	i_am = colt_class_range_list;
}

colt_range_list::~colt_range_list()
{

}

colt_base *colt_range_list::copy(colt_base *op)
{
	return NULL;//new colt_range_list(*op, min_value, max_value);
}

int colt_range_list::add_range(int low, int high)
{
	colt_range *add_new = NULL;//new colt_range(*this, low, high);
	colt_range **tmp;

	if(range_count) {
		int old_size = sizeof(colt_range *)*range_count;
		tmp = (colt_range **) malloc(old_size+sizeof(colt_range *));
		memcpy((void *) tmp, (void *) ranges, old_size);
	} else
		tmp = (colt_range **) malloc(sizeof(colt_range *));

	if(ranges)
		free(ranges);

	ranges = tmp;
	ranges[range_count++] = add_new;

	return range_count;
}
int colt_range_list::process(int rec_num)
{
	for(int i=0; i<range_count; i++)
		if(ranges[i]->in_range(rec_num))
			return colt_operator::process(rec_num);

	return 1;
}
