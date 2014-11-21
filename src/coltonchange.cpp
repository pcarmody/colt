/*
 * coltonchange.cpp
 *
 *  Created on: Sep 7, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltonchange.h"

colt_onchange::colt_onchange(colt_base &in, char *exp, char *col):
	colt_each(in, exp),
	column_number(-1),
	current_value(NULL)
{
	i_am = colt_class_onchange;
	// TODO Auto-generated constructor stub
	column_name = new char[strlen(col)+1];
	strcpy(column_name, col);
	column_name[strlen(col)] = '\0';
}

colt_onchange::~colt_onchange() {
	// TODO Auto-generated destructor stub
}

int colt_onchange::preprocess()
{
	COLT_TRACE("colt_onchange::preprocess()")
	for(int j=0; j<colt_operator::num_cols(); j++) {
		char *head = operand->col_header( j );
		if(strcmp(column_name, head ) == 0 ) {
			column_number = j;
			break;
		}
	}

	if(column_number == -1) {
		cout << "onchange couldn't find columne '" << column_name << "'.\n";
		throw "Could not find column key for onchange.\n";
	}

//	return colt_each::preprocess();
	return 1;
}

int colt_onchange::process(int rec_num)
{
	COLT_TRACE("colt_onchange::process(int rec_num)")
	char **row = fields(rec_num);
	if(!current_value || strcmp(current_value, row[column_number]) != 0) {

		if(expression_object) {
			expression_object->postprocess();
			colt_base *destination = expression_object->get_destination();
			destination->out_object = NULL;
			delete expression_object;
		}

		current_value = row[column_number];

		expression_object = insert_expression(expression_string);
		if(!expression_object->preprocess()) {
			return 0;
		}
	}


	return expression_object->process(rec_num);
}

void colt_onchange::postprocess()
{
	COLT_TRACE("colt_onchange::postprocess()")

	if(expression_object) {
		expression_object->postprocess();
		colt_base *destination = expression_object->get_destination();
		destination->out_object = NULL;
		delete expression_object;
	}

	return;
}
