/*
 * colt_range.cpp
 *
 *  Created on: June 26, 2014
 *      Author: paulcarmody
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include "colt_headers.h"

colt_range::colt_range(char *fname):
	coltthru(fname),
	search_low(NULL),
	search_high(NULL),
	min_value(0),
	max_value(LONG_MAX)
{
	i_am = colt_class_range;
}

colt_range::colt_range(colt_base &in, char *destination_file_name):
	coltthru(in, destination_file_name),
	search_low(NULL),
	search_high(NULL),
	min_value(0),
	max_value(LONG_MAX)
{
	i_am = colt_class_range;
}

colt_range::colt_range(colt_base &b, char *file, char *low, char *high):
	coltthru(b, file),
	search_low(NULL),
	search_high(NULL),
	min_value(0),
	max_value(LONG_MAX)
{
	i_am = colt_class_range;
	search_low = (char *) malloc(strlen(low)+1);
	strcpy(search_low, low);
	search_high = (char *) malloc(strlen(high)+1);
	strcpy(search_high, high);
}

colt_range::colt_range(colt_base &in, char *low, char *high):
	coltthru(in),
	search_low(NULL),
	search_high(NULL),
	min_value(0),
	max_value(LONG_MAX)
{
	i_am = colt_class_range;
	search_low = (char *) malloc(strlen(low)+1);
	strcpy(search_low, low);
	search_high = (char *) malloc(strlen(high)+1);
	strcpy(search_high, high);
}

colt_range::colt_range(colt_base &in, int low, int high):
	coltthru(in),
	search_low(NULL),
	search_high(NULL),
	min_value(low),
	max_value(high)
{
	i_am = colt_class_range;
}

colt_range::~colt_range()
{

}

colt_base *colt_range::copy(colt_base *op)
{
	return new colt_range(*op, search_low, search_high);
}

int colt_range::in_range(int rec_num)
{
	return rec_num >= min_value && rec_num <= max_value;
}

int colt_range::num_lines()
{
	return max_value - min_value - 1;
};

int colt_range::serialize_id_num()
{
	return colt_range_version;
}

int colt_range::write_config(FILE *out)
{
	colt_range_identifier ident;
	ident.id = serialize_id_num();
	ident.min_value = min_value;
	ident.max_value = max_value;
	index_count = 0;

	strcpy(ident.file_name, colt_operator::source_file_name());
	ident.id = colt_range_version;

	fwrite(&ident, 1, sizeof(ident), out);
	return sizeof(ident);
}

int colt_range::show_status(char *baseptr, int indent)
{
	colt_range_identifier ident;

	 memcpy((void *) &ident, (void *) baseptr, sizeof(ident));

	 char indent_str[60];
	 for(int i=0; i<60; i++) indent_str[i] = ' ';
	 indent_str[indent*2] = '\0';

	 std::cout << indent_str << "range:\t" << file_name << "\n";
	 std::cout << indent_str << "min_val:\t" << ident.min_value << "\n";
	 std::cout << indent_str << "max_val:\t" << ident.max_value << "\n";

	 if(match(ident.file_name, "\.thru")) {
		 std::cout << indent_str << "data_source:\n";
		 colt_load_thru(ident.file_name, indent+1);
	 } else
		 std::cout << indent_str << "data_source:" << ident.file_name << "\n";

	 return 1;
}

int *colt_range::read_config(int *base_ptr)
{
	colt_range_identifier ident;

    memcpy((void *) &ident, (void *) index_list, sizeof(ident));

    min_value = ident.min_value;
    max_value = ident.max_value;

	if(match(ident.file_name, "thru$"))
		operand = colt_load_thru(ident.file_name);
	else
		operand = new colt_zsv(ident.file_name, 1);

    operand->out_object = this;

    return base_ptr + sizeof(ident)/sizeof(int);
}

int colt_range::to_string(char *x)
{
	COLT_TRACE("coltthru::to_string(char *x)")
	sprintf(x, "range:%s,%d,%d", source_file_name(), min_value, max_value);
//	char tmp[20];

//	for(int i=0; i<index_count; i++) {
//		sprintf(tmp, ",%d", index_list[i]);
//		strcat(x, tmp);
//	}

	return strlen(x);
}

char *colt_range::from_string(char *input)
{
	char file_name[COLT_MAX_STRING_SIZE];

	sscanf(input, "thru:%s,%d,%d", file_name, &min_value, &max_value);

	while(*input && *input != ',') input++;

	while(*input == ',') {
		*input++;
//		push_back(atoi(input));
		while(*input && *input != ',') input++;
	}

	return input;
}

int colt_range::contains(int rec_num)
{
	return (min_value <= rec_num) && (rec_num <= max_value);
}

int colt_range::num_cols()
{
	return operand->num_cols();
}

char **colt_range::fields(int rec_num)
{
	return operand->fields(rec_num);
}

colt_datatype **colt_range::cells(int rec_num)
{
	return operand->cells(rec_num);
}

char *colt_range::col_header(int i)
{
	return operand->col_header(i);
}

char **colt_range::col_headers()
{
	return operand->col_headers();
}

void colt_range::process_all()
{
	if(!out_object)
		return;

	if(!out_object->preprocess())
		return;

	try {
		for(int index_number=min_value; index_number<=max_value; index_number++) {
//			out_object->process(parent_thru()->index(index_number));
			out_object->process(index_number);
		}
	} catch (colt_exception *e) {

	}

	out_object->postprocess();
}

int colt_range::preprocess()
{
	if(!parent_thru()->preloaded())  // left operand must complete first.
		if(out_object)
			return out_object->preprocess();
		else
			return 1;

	if(search_low && search_high)
		parent_thru()->search(search_low, search_high, this);

	parent_thru()->set_begin_end_index(min_value, max_value);

	return !out_object || out_object->preprocess();
}

int colt_range::process(int rec_num)
{
	if(!out_object)
		return 1;

	if(parent_thru()->preloaded())
		return out_object->process(rec_num);
	return 1;
}

void colt_range::postprocess()
{
	if(parent_thru()->preloaded())
		return coltthru::postprocess();
//	else
//		if(file_name[0])
//			save(file_name);
	coltthru::postprocess();
	parent_thru()->search(search_low, search_high, this);
	cout << "colt_range::postprocess " << search_low << ":" << search_high << "\n";
	cout << "colt_range::postprocess " << min_value << ":" << max_value << "\n";
	parent_thru()->set_begin_end_index(min_value, max_value);

	process_all();

}

