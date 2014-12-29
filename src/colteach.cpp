/*
 * colteach.cpp
 *
 *  Created on: Aug 15, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "colteach.h"

colt_each::colt_each(colt_base &in, char *exp):
	colt_operator(in),
	expression_string(NULL),
	expression_object(NULL),
	destination(NULL)
{
	i_am = colt_class_each;
	// TODO Auto-generated constructor stub
	expression_string = new char[strlen(exp)+1];
	strcpy(expression_string, exp);
}

colt_each::~colt_each()
{
	// TODO Auto-generated destructor stub
	delete expression_string;
}

colt_operator *colt_each::insert_expression(char *expression, int rec_num, int no_destination)
{
	COLT_TRACE("*colt_each::insert_expression(char *expression)")
	colt_operator *retval;
	colt_parser parse(expression, cells(rec_num), col_headers(), num_cols());


	retval = new colt_operator;
	retval->operand = operand;

	parse.parse_unary_expressions(retval);

	if(!no_destination)
		retval->set_destination(out_object, 0);

	return retval;
}

int colt_each::process(int rec_num)
{
	COLT_TRACE("colt_each::process(int rec_num)")
	expression_object = insert_expression(expression_string, rec_num);

	if(!expression_object->preprocess()) {
		return 0;
	}

	_trace.start() << "ready to process subexpression." << expression_object->i_am << "\n";
	expression_object->process(rec_num);

	expression_object->postprocess();

	if(expression_object) {

		colt_base *destination = expression_object->get_destination();
		destination->out_object = NULL;
		delete expression_object;
	}

	return 1;
}
