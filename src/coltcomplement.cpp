/*
 * coltcomplement.cpp
 *
 *  Created on: Aug 9, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltcomplement.h"

colt_complement::colt_complement(colt_base &in, coltbitmap *r):
	colt_isect(in, r)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_complement;
}

colt_complement::~colt_complement() {
	// TODO Auto-generated destructor stub
}

void colt_complement::generate(coltbitmap *left_operand)
{
	if(left_operand->max_value < right_operand->min_value
	|| left_operand->min_value > right_operand->max_value)
		return;	// empty set nothing to process

	allocate(left_operand->min_value, left_operand->max_value);

	for(int i=0; i<num_elements; i++)
		map[i] = left_operand->map[i];

	int displace = (right_operand->initial_disp - initial_disp) / COLT_MAP_BITS_IN_ELEM;
	long xxx = 0;

	for(int i=0; i<right_operand->num_elements; i++) {
		map[displace+i] &= ~(map[displace+i] & right_operand->map[i]);
		if(map[displace+i] && !xxx)
			xxx = map[displace+i];
	}
	if(!xxx)
		free(map);
}
