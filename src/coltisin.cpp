/*
 * coltisin.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltisin.h"

colt_isin::colt_isin(colt_base &in, char *exp, int neg):
	colt_each(in, exp),
	negative(neg)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_isin;
}

colt_isin::~colt_isin() {
	// TODO Auto-generated destructor stub
}

int colt_isin::process(int rec_num)
{
	COLT_TRACE("colt_isin::process(int rec_num)")

	colt_parser parse(expression_string, cells(rec_num), col_headers(), num_cols());

	colt_base *expression = parse.parse(1);

	cout << "qqq " << index_file_name() << " <=> " << expression->source_file_name() << " <+> " << expression->index_file_name() << "\n";

	if(!expression->preprocess()) {
		return 0;
	}

	int retval = 1;

	if(negative) {
		if(!expression->contains(rec_num))
			retval = colt_operator::process(rec_num);
	} else {
		if(expression->contains(rec_num))
			retval = colt_operator::process(rec_num);
	}
//	_trace.start() << "ready to process subexpression." << expression_object->i_am << "\n";
//	expression_object->process(rec_num);
//
//	expression_object->postprocess();

	if(expression) {

//		colt_base *destination = expression->get_destination();
//		destination->out_object = NULL;
		delete expression;
	}

	return retval;
}
