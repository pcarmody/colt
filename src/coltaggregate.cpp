/*
 * coltaggregate.cpp
 *
 *  Created on: Aug 21, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "stdlib.h"
#include "coltaggregate.h"

colt_aggregate::colt_aggregate(colt_base &in):
	colt_operator(in),
	agg_details(),
	result_fields(NULL),
	values(NULL)
{
// TODO Auto-generated constructor stub
	i_am = colt_class_aggregate;
}

colt_aggregate::colt_aggregate(colt_base &in, int func, char *col_name):
	colt_operator(in),
	agg_details(),
	result_fields(NULL),
	values(NULL)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_aggregate;
	add_aggregation(func, col_name);
}

colt_aggregate::~colt_aggregate()
{
	// TODO Auto-generated destructor stub
	delete result_fields;
	delete values;
	for(int i=0; i<agg_details.size(); i++)
		delete agg_details[i];
}

void colt_aggregate::add_aggregation(int func, char *col_name, int col_number)
{
	agg_details.push_back(new agg_details_struct(func, col_name, col_number));
}


char **colt_aggregate::fields(int rec_num)
{
	if(rec_num < 0) {
		return result_fields;
	}
	return colt_operator::fields(rec_num);
}

int colt_aggregate::preprocess()
{
//	for ( vector<agg_details_struct *>::iterator i = agg_details.begin(); i != agg_details.end(); i++ )  {
	for(int i=0; i<agg_details.size(); i++) {
		for(int j=0; j<colt_operator::num_cols(); j++) {
			char *head = operand->col_header( j );
			if(strcmp(agg_details[i]->column_name, head ) == 0 ) {
				agg_details[i]->column_number = j;
				break;
			}
		}

		if(agg_details[i]->column_number == -1)
			throw "Could not find column key for aggregation.\n";
	}

	result_fields = (char **) malloc(sizeof(char *) * num_cols());
	values = (long *) malloc(sizeof(long) * num_cols());

	for(int j=0; j<agg_details.size(); j++)
		for(int i=0; i<num_cols(); i++) {
			result_fields[i] = NULL;
			if(agg_details[j]->function == min)
				values[i] = LONG_MAX;
			else
				values[i] = 0;
		}

	return colt_operator::preprocess();
}

int colt_aggregate::process(int rec_num)
{
	char **f = fields(rec_num);

	for(int j=0; j<agg_details.size(); j++) {
		int i=agg_details[j]->column_number;
		if(agg_details[j]->function == sum)
			values[i] += atol(f[i]);
		else if(agg_details[j]->function == count)
			values[i] += 1;
		else if(agg_details[j]->function == first) {
			if(values[i] == 0)
				values[i] = (long) f[i];
		} else if(agg_details[j]->function == last) {
			values[i] = (long) f[i];
		} else if(agg_details[j]->function == max) {
			int num = atol(f[i]);
			if(num > values[i])
				values[i] = num;
		} else if(agg_details[j]->function == min) {
			int num = atol(f[i]);
			if(num < values[i])
				values[i] = num;
		}
	}

	for(int j=0; j<num_cols(); j++)
		result_fields[j] = f[j];

	return 1;
}

void colt_aggregate::postprocess()
{
	for(int j=0; j<agg_details.size(); j++) {
		int i=agg_details[j]->column_number;
		if(agg_details[j]->function == first || agg_details[j]->function == last)
			result_fields[i] = (char *) values[i];
		else {
			result_fields[i] = new char[10];
			sprintf(result_fields[i], "%d", values[i]);
		}
	}
	out_object->process(-1);

	for(int j=0; j<agg_details.size(); j++) {
		int i=agg_details[j]->column_number;
		if(agg_details[j]->function != first && agg_details[j]->function != last)
			delete result_fields[i];
	}

	out_object->postprocess();
}
