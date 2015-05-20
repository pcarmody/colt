/*
 * colt_cross.cpp
 *
 *  Created on: June 24, 2014
 *      Author: paulcarmody
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "colt_headers.h"
#include "colt_cross.h"

colt_cross::colt_cross(colt_base&in, char *exp):
	colt_each(in, exp),
	current_rec_num(-1),
	field_list(NULL),
	cross_headers(NULL),
	left_num_cols(0),
	right_num_cols(0)
{
	i_am = colt_class_cross;
}

colt_cross::~colt_cross()
{
	if(field_list)
		free(field_list);

	if(cross_headers)
		free(cross_headers);
}

colt_base	*colt_cross::copy(colt_base *op)
{
	COLT_TRACE("*colt_cross::copy(colt_base *op)")
//	return new colt_cross(*op, &expression_object.operand->copy(NULL));
	return this;
}

colt_operator *colt_cross::insert_expression(char *expression, int rec_num, int no_destination)
{
	COLT_TRACE("*colt_cross::insert_expression(char *expression)")
	colt_operator *retval;
	int cols = num_cols();
	char **heads = col_headers();

	colt_parser parse(expression, cells(rec_num), heads, cols);

	retval = (colt_operator *) parse.parse(1);

	if(!no_destination)
		retval->set_destination(out_object, 0);

	return retval;
}

int colt_cross::num_cols()
{
	COLT_TRACE("colt_cross::num_cols()")
//	if(!expression_object)
//		return colt_each::num_cols();
//
//	return colt_each::num_cols() + expression_object->num_cols();
	return left_num_cols + right_num_cols;
}

char *colt_cross::col_header(int n)
{
	COLT_TRACE("*colt_cross::col_header(int n)")
	if(n < left_num_cols)
		return colt_each::col_header(n);

	if(!cross_headers)
		col_headers();

	return cross_headers[n];
//	return expression_object->col_header(n - left_cols);
}

char **colt_cross::col_headers()
{
	if(cross_headers)
		return cross_headers;

	char **left_headers = colt_operator::col_headers();

	if(right_num_cols == 0)
		return left_headers;

	if(!expression_object)
		expression_object = xrefs.nth(xrefs.Size()-1)->expression;

	char **right_headers = expression_object->col_headers();

	cross_headers = (char **) malloc(sizeof(char *) * (left_num_cols+right_num_cols));
	int i=0;
	for(i=0; i<left_num_cols; i++)
		cross_headers[i] = left_headers[i];
	for(int j=0; j<right_num_cols; j++) {
		char *tmp = new char[COLT_MAX_STRING_SIZE];
		sprintf(tmp, "%s.%s", expression_object->source_file_name(),right_headers[j]);
		cross_headers[i+j] = tmp; //right_headers[j];
	}

	return cross_headers;
}

int colt_cross::compare(int a, int b, int c)
{
	colt_cross_xrefs *xrefa = xrefs[a];
	colt_cross_xrefs *xrefb = xrefs[b];

	if(c < left_num_cols)
		return colt_each::compare(xrefa->left, xrefb->left, c);

	colt_datatype *left= xrefa->expression->cells(xrefa->right)[c-left_num_cols];
	colt_datatype *rite= xrefb->expression->cells(xrefb->right)[c-left_num_cols];

	return left->compare(*rite);

//	return xrefa->expression->compare(xrefa->right, xrefb->right, c-left_num_cols);
}

char **colt_cross::fields(int rec_num)
{
	COLT_TRACE("**colt_cross::fields(int rec_num)")
	colt_cross_xrefs *xref = xrefs[rec_num];
	char **left = colt_each::fields(xref->left);

	if(!xref)
		return left;

	char **rite = xref->expression->fields(xref->right);

//	if(field_list) {
//		free(field_list);
//		field_list = NULL;
//	}

	if(!field_list)
		field_list = (char **) malloc(sizeof(char *) * num_cols());

	int i;
	for(i=0; i<left_num_cols; i++)
		field_list[i] = left[i];
	for(int j=0; j<right_num_cols; j++)
		field_list[i++] = rite[j];

	return field_list;
}

colt_datatype **colt_cross::cells(int rec_num)
{
	COLT_TRACE("**colt_cross::cells(int rec_num)")
	colt_datatype **left = colt_each::cells(current_rec_num);

	if(!expression_object)
		return left;

	colt_datatype **rite = expression_object->cells(rec_num);

	if(!cell_list)
		cell_list = (colt_datatype **) malloc(sizeof(colt_datatype *) * num_cols());

	int i;
	for(i=0; i<left_num_cols; i++)
		cell_list[i] = left[i];
	for(int j=0; j<right_num_cols; j++)
		cell_list[i++] = rite[j];

	return cell_list;
}

int	colt_cross::preprocess()
{
	if(expression_object)
		return 1;

	if(!left_num_cols)
		left_num_cols = colt_each::num_cols();
	return colt_each::preprocess();
}

int colt_cross::process(int rec_num)
{
	COLT_TRACE("colt_cross::process(int rec_num)")
	static colt_base *tmp_op;

	if(expression_object) {
		operand = tmp_op;
		int curr = xrefs.Size();
		xrefs.AddItem(curr, new colt_cross_xrefs(current_rec_num, rec_num, expression_object));
		return colt_operator::process(curr);
	}
	current_rec_num = rec_num;
	tmp_op = operand;
	expression_object = insert_expression(expression_string, rec_num, 1);

	right_num_cols = expression_object->num_cols();
	colt_base *destination = expression_object->get_destination();

	expression_object->set_destination(this);

//	expression_object->process_all();
	int rec;
	if(!expression_object->preprocess())
		return 0;

	try {
		while((rec = expression_object->get_next_row()) >= 0) {
			expression_object->process(rec);
		}
	} catch (colt_exception *e) {

	}
//	destination->out_object = NULL;

//	expression_object->postprocess();


	expression_object = NULL;
	operand = tmp_op;

	return 1;
}
