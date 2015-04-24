/*
 * coltindex.cpp
 *
 *  Created on: Apr 20, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltindex.h"

colt_index::colt_index(colt_base &in, int c, int *l):
	colt_operator(in),
	count(c),
	index_list(NULL)
{
	// TODO Auto-generated constructor stub

	count = c;
	index_list = new int[count];
	for(int i=0; i<count; i++)
		index_list[i] = l[i];

}

colt_index::~colt_index() {
	// TODO Auto-generated destructor stub

	free(index_list);
}

int colt_index::process(int row)
{
	for(int i=0; i<count; i++)
		if(!colt_operator::process(index_list[i]))
			return 0;

	throw "valid";

	return 0;
}
