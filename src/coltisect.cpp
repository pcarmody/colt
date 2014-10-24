/*
 * coltisect.cpp
 *
 *  Created on: Aug 4, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltisect.h"

colt_isect::colt_isect(colt_base &in, colt_base *r):
	coltbitmap(in)
{
	i_am = colt_class_isect;
}

colt_isect::colt_isect(colt_base &b, char *file, coltbitmap *r):
	coltbitmap(b, file),
	right_operand(r)
{
	i_am = colt_class_isect;
}

colt_isect::~colt_isect() {
	// TODO Auto-generated destructor stub
}

void colt_isect::generate(coltbitmap *left_operand)
{
	if(left_operand->max_value < right_operand->min_value
	|| left_operand->min_value > right_operand->max_value)
		return;	// empty set nothing to process

	min_value = (left_operand->min_value > right_operand->min_value)?
			left_operand->min_value:
			right_operand->min_value;
	max_value = (left_operand->max_value < right_operand->max_value)?
			left_operand->max_value:
			right_operand->max_value;

	int left_start = (initial_disp - left_operand->initial_disp) / COLT_MAP_BITS_IN_ELEM;
	int right_start = (initial_disp - right_operand->initial_disp) / COLT_MAP_BITS_IN_ELEM;

	if(left_operand->all_bits_set() && right_operand->all_bits_set())
		return; 	// reduced to a range.
	else if(left_operand->all_bits_set()) {

		for(int i=0; i<num_elements; i++)
			map[i] =  right_operand->map[right_start+i];
		return;

	} else if(right_operand->all_bits_set()) {

		for(int i=0; i<num_elements; i++)
			map[i] = left_operand->map[left_start+i];
		return;

	}

	allocate(min_value, max_value);

	for(int i=0; i<num_elements; i++) {
		map[i] = left_operand->map[left_start+i] & right_operand->map[right_start+i];
	}

}

void colt_isect::process_all()
{
	generate((coltbitmap *) operand);
	coltbitmap::process_all();
}
