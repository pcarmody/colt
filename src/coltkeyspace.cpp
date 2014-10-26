/*
 * coltkeyspace.cpp
 *
 *  Created on: Aug 13, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltkeyspace.h"

colt_keyspace::colt_keyspace(colt_base &in, int col, char *destination_file_name):
	colt_sort(in, destination_file_name, (char *) NULL),
//	colt_sort(in, 0),
//	column_number(col),
	column_value(NULL)
{
	i_am = colt_class_thru;
	column_number = col;
}

colt_keyspace::colt_keyspace(char *source):
	colt_sort(source),
	column_value(NULL)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_thru;
}

colt_keyspace::~colt_keyspace()
{
	// TODO Auto-generated destructor stub
}

int colt_keyspace::serialize_id_num()
{
	return colt_keyspace_version;
}

int colt_keyspace::num_cols()
{
	return 1;
}

char *colt_keyspace::col_header(int n)
{
	return colt_operator::col_header(column_number);
}

char **colt_keyspace::fields(int rec_num)
{
	column_value = &(operand->fields(index_list[rec_num])[column_number]);
	return column_value;
}

int colt_keyspace::preprocess()
{
	if(preloaded())
		return 1;//	out_object && out_object->preprocess();

	int retval = colt_sort::preprocess();
	push_back(0);

	return retval;
}

int colt_keyspace::process(int rec_num)
{
	if(preloaded()) {
		return out_object->process(rec_num);
//		int low = index_list[rec_num];
//		int high = index_list[rec_num + 1] - 1;
//		for(int i=low; i<=high; i++)
//			out_object->process(i);
//
//		return 1;
	}

	char *key_value = operand->fields(index_list[index_count-1])[column_number];
	char *current_value = operand->fields(rec_num)[column_number];

	if(strcmp(key_value, current_value) != 0)
		push_back(rec_num);

	return 1;
}

void colt_keyspace::perform_sort()
{
	push_back(operand->num_lines());
	return;
}

