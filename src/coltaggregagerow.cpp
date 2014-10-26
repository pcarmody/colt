/*
 * coltaggregagerow.cpp
 *
 *  Created on: Aug 22, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltaggregagerow.h"

colt_aggregate_row::colt_aggregate_row(colt_base &in, int func, int cnt, char *col):
	colt_operator(in),
	function(func),
	count(cnt),
	column_name(NULL),
	column_number(-1)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_aggregate_row;
	rows = new int[cnt];
	for(int i=0; i<cnt; i++) rows[i] = -1;
	column_name = new char[strlen(col)+1];
	strcpy(column_name, col);
	column_name[strlen(col)] = '\0';
}

colt_aggregate_row::~colt_aggregate_row()
{
	delete rows;
}

int colt_aggregate_row::preprocess()
{
	for(int j=0; j<colt_operator::num_cols(); j++) {
		char *head = operand->col_header( j );
		if(strcmp(column_name, head ) == 0 ) {
			column_number = j;
			break;
		}
	}

	if(column_number == -1)
		throw "Could not find column key for aggrow.\n";

	return colt_operator::preprocess();
}
int colt_aggregate_row::process(int rec_num)
{
	char **f;
	if(function != colt_aggregate::first && function != colt_aggregate::last)
		f= fields(rec_num);

	if(function == colt_aggregate::first) {
		if(rows[count-1] < 0) {
			int i;
			for(i=0; i<count && rows[i] >= 0; i++);
			rows[i] = rec_num;
		}
	} else if(function == colt_aggregate::last) {
		for(int i=count-1; i > 0; i--) {
			rows[i] = rows[i-1];
		}
		rows[0] = rec_num;
	} else if(function == colt_aggregate::max) {
		char *num = f[column_number];
		if(rows[0] < 0) {
			rows[0] = rec_num;
			return 1;
		}
		if(rows[count-1] < 0 ||
				strcmp(num, fields(rows[count-1])[column_number]) < 0) {
			for(int i=0; i<count; i++) {
				char **g = fields(rows[i]);
				char *curr = g[column_number];
				if(strcmp(curr, num) < 0) {
					num = curr;
					int tmp = rec_num;
					rec_num = rows[i];
					rows[i] = tmp;
				}
			}
		}
	} else if(function == colt_aggregate::min) {
		char *num = f[column_number];
		if(rows[count-1] < 0 ||
				strcmp(num, fields(rows[count-1])[column_number]) < 0) {
			for(int i=0; i<count; i++) {
				if(rows[i] < 0) {
					rows[i] = rec_num;
					break;
				}
				char **g = fields(rows[i]);
				char *curr = g[column_number];
				if(strcmp(curr, num) > 0) {
					num = curr;
					int tmp = rec_num;
					rec_num = rows[i];
					rows[i] = tmp;
				}
			}
		}
	}
//	} else if(function == colt_aggregate::min) {
//		if(rows[i] == -1)
//			rows[i] = rec_num;
//		else {
//			long num = atol(f[column_number]);
//			char **g = fields(rows[i]);
//			long curr = atol(g[column_number]);
//			if(num < curr)
//				rows[i] = rec_num;
//		};
//	}

	return 1;
}

void colt_aggregate_row::postprocess()
{
	for(int i=0; i<count; i++)
		out_object->process(rows[i]);

	out_object->postprocess();
}

