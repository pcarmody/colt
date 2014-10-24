/*
 * colt_cross.cpp
 *
 *  Created on: June 24, 2014
 *      Author: paulcarmody
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "coltbase.h"
#include "coltoperator.h"
#include "colt_cross.h"

colt_cross::colt_cross(colt_base &in, colt_base &rite):
	colt_operator(in),
	right(&rite),
	current_rec_num(-1),
	field_list(NULL)
{
	i_am = colt_class_cross;
}

colt_cross::~colt_cross()
{
	if(field_list)
		free(field_list);
}

colt_base	*colt_cross::copy(colt_base *op)
{
//	return new colt_cross(*op, &right.operand->copy(NULL));
	return this;
}

int colt_cross::num_cols()
{
	return colt_operator::num_cols() + right->num_cols();
}

char *colt_cross::col_header(int n)
{
	int left_cols = colt_operator::num_cols();
	if(n < left_cols)
		return colt_operator::col_header(n);
	return right->col_header(n - left_cols);
}

char **colt_cross::fields(int rec_num)
{
	char **left = colt_operator::fields(current_rec_num);
	char **rite = right->fields(rec_num);

	if(field_list) {
		free(field_list);
		field_list = NULL;
	}

	field_list = (char **) malloc(sizeof(char *) * num_cols());

	int i;
	for(i=0; i<colt_operator::num_cols(); i++)
		field_list[i] = left[i];
	for(int j=0; j<right->num_cols(); j++)
		field_list[i++] = rite[j];

	return field_list;
}

int colt_cross::process(int rec_num)
{
	if(!right->preprocess())
		return 0;

	current_rec_num = rec_num;

//	for(int i=0; i<right->num_lines(); i++)
	int i;
	while((i = right->get_next_row()) >= 0)
		if(!colt_operator::process(i))
			return 0;

	return 1;
}
