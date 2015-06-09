/*
 * coltreducethru.cpp
 *
 *  Created on: Jun 9, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltreducethru.h"

colt_reduce_thru::colt_reduce_thru(colt_base &in, char *fname) :
	colt_operator(in),
	thru(NULL)
{
	// TODO Auto-generated constructor stub
	thru = colt_load_thru(fname);
}

colt_reduce_thru::~colt_reduce_thru() {
	// TODO Auto-generated destructor stub
}

int colt_reduce_thru::process(int rec_num)
{
	return colt_operator::process(thru->reduce_to(0, rec_num));
}
