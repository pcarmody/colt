/*
 * coltaddsource.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltaddsource.h"

colt_add_source::colt_add_source(colt_base &in, char *col_name, int t, char *tstr):
	colt_add(in, col_name, t, tstr)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_add_source;

	strcpy(code_string, value);}

colt_add_source::~colt_add_source() {
	// TODO Auto-generated destructor stub
}

void colt_add_source::get_value(int rec_num)
{
	int cols = num_cols();

	if(elements.IsItem(rec_num)) {
		colt_datatype *new_cell = colt_add_cell[cols-1];
		new_cell->set_value(elements[rec_num]);
		new_cell->format(value);
	}
	colt_datatype **rec = colt_operator::cells(rec_num);

	if(!rec)
		return;

	colt_parser parse(code_string, rec, col_headers(), cols-1);

	colt_base *exp_object = parse.parse(1);

	exp_object->process_all();

	coltthru *thru = (coltthru *) exp_object->get_destination();

	colt_datatype *new_cell = colt_add_cell[cols-1];

	if(thru->is_a(colt_class_cthru))
		new_cell->set_type(COLT_DT_CTHRU);
	else if(thru->is_a(colt_class_sort))
		new_cell->set_type(COLT_DT_SORT);
	else if(thru->is_a(colt_class_thru))
		new_cell->set_type(COLT_DT_THRU);
	else if(thru->is_a(colt_class_range))
		new_cell->set_type(COLT_DT_RANGE);
	else if(thru->is_a(colt_class_bitmap))
		new_cell->set_type(COLT_DT_BITMAP);
	else {
		perror("Add column must add a thru object.\n");
		exit(1);
	}

	elements.AddItem(rec_num, thru);
//	new_cell->set_value(thru);
	new_cell->value_type = (colt_datatype::value_type_t *) thru;
	new_cell->format(value);
	if(colt_add_cell[cols-1])
		colt_add_cell[cols-1]->value_type = (colt_datatype::value_type_t *) thru;
//		colt_add_cell[cols-1]->set_value(thru);
}
