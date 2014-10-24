/*
 * coltpartition.cpp
 *
 *  Created on: Aug 19, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltpartition.h"

colt_partition::colt_partition(colt_base &in, char *exp, char *col):
	colt_onchange(in, exp, col),
	list()
{
	i_am = colt_class_partition;
}

colt_partition::~colt_partition() {
	// TODO Auto-generated destructor stub
}

colt_operator *colt_partition::insert_expression(char *expression)
{
	colt_operator *retval;
	colt_parser parse(expression);

	retval = new colt_operator;
	retval->operand = operand;

	parse.parse_unary_expressions(retval);

//	retval->set_destination(out_object, 0);

	return retval;
}

int colt_partition::process(int rec_num)
{
	int retval = 0;
	char **row = fields(rec_num);
	char *key = row[column_number];
	colt_operator *expression = NULL;
	colt_operator *endl = NULL;

	if(!list.IsItem(key)) {

		expression = insert_expression(expression_string);
		endl = (colt_operator *) expression->get_destination();
		endl->set_destination(out_object, 1);

		if(!expression->preprocess())
			return 0;

		endl->out_object = NULL;
		list.AddItem(key, expression);
		end_of_list.AddItem(key, endl);

	} else {
		expression = list[key];
		endl = end_of_list[key];
	}

	endl->set_destination(out_object, 1);
	expression->process(rec_num);
	endl->out_object = NULL;

	return retval;
}

void colt_partition::postprocess()
{
	for(int i=0; i<list.Size(); i++) {
		end_of_list[i]->set_destination(out_object, 1);
		list[i]->postprocess();
		end_of_list[i]->out_object = NULL;
	}

	return colt_operator::postprocess();
}
