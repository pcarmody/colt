/*
 * coltcounter.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltcounter.h"

colt_counter::colt_counter(colt_base &n):
	colt_out(n),
	counter(0)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_counter;
}

colt_counter::~colt_counter() {
	// TODO Auto-generated destructor stub
}

int colt_counter::preprocess()
{
	return 1;//out_object && out_object->preprocess();
}

int colt_counter::process(int rec_num)
{
	cout << counter++ << " lines\r";
	return 1;
}

void colt_counter::postprocess()
{
	cout << "\n";
}
