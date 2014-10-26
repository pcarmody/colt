/*
 * coltunion.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltunion.h"

colt_union::colt_union(colt_base &in, colt_base *r):
	colt_isect(in, r)
{
	i_am = colt_class_union;
}

colt_union::colt_union(colt_base &b, char *file, coltbitmap *r):
	colt_isect(b, file, r)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_union;
}

colt_union::~colt_union() {
	// TODO Auto-generated destructor stub
}

void colt_union::generate(coltbitmap *left_operand)
{
//	left_operand = (coltbitmap *) operand;
	coltbitmap *tmp = right_operand;

	if(left_operand->min_value > right_operand->min_value) {
		right_operand = left_operand;
		left_operand = tmp;
	}

	min_value = left_operand->min_value;
	max_value = (left_operand->max_value > right_operand->max_value)?
			left_operand->max_value:
			right_operand->max_value;

	if(left_operand->max_value > right_operand->min_value)  // they intersect
		if(left_operand->all_bits_set() && right_operand->all_bits_set())
			return;		// generate a range instead of a map.

	allocate(min_value, max_value);

	for(int i=0; i<left_operand->num_elements; i++)
		map[i] = left_operand->map[i];

	int displace = (right_operand->initial_disp - initial_disp) / COLT_MAP_BITS_IN_ELEM;

	for(int i=0; i<right_operand->num_elements; i++)
		map[displace+i] |= right_operand->map[i];

	right_operand = tmp;

	coltbitmap::process_all();
}
