/*
 * coltlink.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltlink.h"

colt_link::colt_link(colt_base &in, char *colname, char *exp):
	colt_each(in, exp),
	column_name(NULL)
{
	// TODO Auto-generated constructor stub
}

colt_link::~colt_link()
{
	// TODO Auto-generated destructor stub
}

int colt_link::process(int rec_num)
{
	COLT_TRACE("colt_link::process(int rec_num)")
	colt_parser parse(expression_string);

	colt_base *exp_object = parse.parse();

	exp_object->process_all();

	return colt_operator::process(rec_num);
}
