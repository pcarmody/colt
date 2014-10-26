/*
 * coltshare.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltshare.h"

colt_share::colt_share(colt_base &in):
	colt_operator(in),
	expressions()
{
	i_am = colt_class_share;
	// TODO Auto-generated constructor stub

}

colt_share::~colt_share() {
	// TODO Auto-generated destructor stub
	for(int i=0; i<expressions.size(); i++)
		free(expressions[i]);
}

void colt_share::add_expression(colt_operator *exp)
{
	expressions.push_back(exp);
	out_object = NULL;
}

int colt_share::preprocess()
{
	int retval = colt_operator::preprocess();
	for(int i=0; i<expressions.size(); i++) {
		expressions[i]->operand = operand;
		expressions[i]->preprocess();
	}

	return retval;
}

int colt_share::process(int rec_num)
{
	for(int i=0; i<expressions.size(); i++) {\
		colt_base *end_point = expressions[i]->get_destination();
		end_point->out_object = out_object;
		((colt_operator *) out_object)->operand = end_point;
		expressions[i]->process(rec_num);
		end_point->out_object = NULL;
		((colt_operator *) out_object)->operand = this;
	}

	return 1;
}

void colt_share::postprocess()
{
	for(int i=0; i<expressions.size(); i++)
		expressions[i]->postprocess();

	return colt_operator::postprocess();
}
