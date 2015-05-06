/*
 * coltbitmap.cpp
 *
 *  Created on: Jul 28, 2014
 *      Author: paul
 */

#include "colt_headers.h"

coltbitmap::coltbitmap(char *fname):
	coltthru(fname),
	map(NULL),
	num_elements(0),
	initial_disp(0),
	min_value(0),
	max_value(LONG_MAX)
{
	i_am = colt_class_bitmap;
}

coltbitmap::coltbitmap(colt_base &in, char *destination_file_name, int all_bits):
	coltthru(in, destination_file_name),
	map(NULL),
	num_elements(0),
	initial_disp(0),
	min_value(0),
	max_value(LONG_MAX)
{
	i_am = colt_class_bitmap;
}

coltbitmap::coltbitmap(colt_base &b, char *file, char *low, char *high, int all_bits):
	coltthru(b, file),
	map(NULL),
	num_elements(0),
	initial_disp(0),
	min_value(0),
	max_value(LONG_MAX)
{
	i_am = colt_class_bitmap;
	if(max_value != LONG_MAX && !all_bits) {
		allocate(min_value, max_value);
	}
}

coltbitmap::coltbitmap(colt_base &in, char *low, char *high, int all_bits):
	coltthru(in),
	map(NULL),
	num_elements(0),
	initial_disp(0),
	min_value(0),
	max_value(-1)
{
	i_am = colt_class_bitmap;
	if(max_value != LONG_MAX && !all_bits) {
		allocate(min_value, max_value);
	}
}

coltbitmap::~coltbitmap()
{
	if(!file_name[0] && map)
		free(map);
}

//typedef char[4] colt_long;

int	coltbitmap::lowest_bit_set(int start)
{
	int i=0;
	if(start == -1)
		start = initial_disp;
	else {
		i = (start - initial_disp) / COLT_MAP_BITS_IN_ELEM;
		int disp = start % COLT_MAP_BITS_IN_ELEM;
//		if(disp) {
			for(int j=disp; j<COLT_MAP_BITS_IN_ELEM; j++)
				if(map[i] & (COLT_MAP_HIGH_BIT >> j))
					return start;
				else
					start += 1;
//		}
		i++;
	}
	while(i < num_elements) {
		if(map[i] == 0)
			start += COLT_MAP_BITS_IN_ELEM;
		else {
			if(map[i] & COLT_MAP_HIGH_BIT)
				return start;
//			start += 1;
			for(int j=1; j<COLT_MAP_BITS_IN_ELEM; j++)
				if(map[i] & (COLT_MAP_HIGH_BIT >> j))
					return start+1;
				else
					start += 1;
		}
		i++;
	}
	return -1;
}

int	coltbitmap::highest_bit_set(int start)
{
	int i=0;
	if(start == -1)
		start = initial_disp;
	else {
		i = (start - initial_disp) / COLT_MAP_BITS_IN_ELEM;
		int disp = start % COLT_MAP_BITS_IN_ELEM;
//		if(disp) {
			for(int j=disp; j<COLT_MAP_BITS_IN_ELEM; j++)
				if(map[i] & (COLT_MAP_HIGH_BIT >> j))
					start += 1;
				else
					return start-1;
//		}
		i++;
	}
	while(i < num_elements) {
		if(map[i] == -1)
			start += COLT_MAP_BITS_IN_ELEM;
		else {
			for(int j=0; j<COLT_MAP_BITS_IN_ELEM; j++)
				if(map[i] & (COLT_MAP_HIGH_BIT >> j))
					start += 1;
				else
					return start-1;
		}
		i++;
	}
	return start-1;
}

void coltbitmap::xlate_to_range_list()
{
	int low = 0;
	int high = -1;
	while((low = lowest_bit_set(high)) >= 0) {
		high = highest_bit_set(low);
		// add low-high range to the list
		high++;
	}
}

int coltbitmap::serialize_id_num()
{
	return colt_bitmap_version;
}

int coltbitmap::write_config(FILE *out)
{
	colt_bitmap_identifier ident;
	ident.id = serialize_id_num();
	ident.min_value = min_value;
	ident.max_value = max_value;
	ident.num_elements = num_elements;
	index_count = 0;

	strcpy(ident.file_name, colt_operator::source_file_name());

	fwrite(&ident, 1, sizeof(ident), out);
	fwrite((char *) map, num_elements, sizeof(long), out);
	return sizeof(ident);
}

int *coltbitmap::read_config(int *base_ptr)
{
	colt_bitmap_identifier ident;

    memcpy((void *) &ident, (void *) index_list, sizeof(ident));

    min_value = ident.min_value;
    max_value = ident.max_value;
//    allocate(ident.min_value, ident.max_value);
	int minv = min_value / COLT_MAP_BITS_IN_ELEM;
    initial_disp = minv * COLT_MAP_BITS_IN_ELEM;
	int maxv = (max_value / COLT_MAP_BITS_IN_ELEM) + 1;
	num_elements = maxv-minv;

//    free(map);
    map = (long *) ((char *) index_list + sizeof(ident));

	if(match(ident.file_name, "thru$"))
		operand = colt_load_thru(ident.file_name);
	else
		operand = new colt_zsv(ident.file_name, 1);

//	show();

    operand->out_object = this;

    return base_ptr + sizeof(ident)/sizeof(int);
}

int coltbitmap::to_string(char *x)
{
	COLT_TRACE("coltbitmap::to_string(char *x)")
	char tmp[COLT_MAX_STRING_SIZE];
	char tmp2[20];

	tmp[0] = '\0';

	for(int i=0; i<num_elements; i++) {
		sprintf(tmp2, "%04x", map[i]);
		strcat(tmp, tmp2);
	}

	sprintf(x, "bitmap:%s,%d,%d,%s", source_file_name(), min_value, max_value, tmp);
//	char tmp[20];
//
//	for(int i=0; i<index_count; i++) {
//		sprintf(tmp, ",%d", index_list[i]);
//		strcat(x, tmp);
//	}

	return strlen(x);
}

char *coltbitmap::from_string(char *input)
{
	COLT_TRACE("coltbitmap::from_string(char *intpu)")
	char file_name[COLT_MAX_STRING_SIZE];

	sscanf(input, "thru:%s,", file_name);

	while(*input && *input != ',') input++;

	while(*input == ',') {
		*input++;
		push_back(atoi(input));
		while(*input && *input != ',') input++;
	}

	return input;
}

int coltbitmap::contains(int rec_num)
{
	return  is_set(rec_num);
}

int coltbitmap::show_status(char *baseptr, int indent)
{
//	coltthru_identifier ident;
//
//	 memcpy((void *) &ident, (void *) baseptr, sizeof(ident));
//
//	 char indent_str[60];
//	 for(int i=0; i<60; i++) indent_str[i] = ' ';
//	 indent_str[indent*2] = '\0';
//
//	 std::cout << indent_str << "bitmap:\t" << file_name << "\n";
//	 std::cout << indent_str << "min_val:\t" << ident.min_value << "\n";
//	 std::cout << indent_str << "max_val:\t" << ident.max_value << "\n";
//
////	 show();
//	 if(match(ident.file_name, "\.thru")) {
//		 std::cout << indent_str << "data_source:\n";
//		 colt_load_thru(ident.file_name, indent+1);
//	 } else
//		 std::cout << indent_str << "data_source:" << ident.file_name << "\n";

	 return 1;
}

void coltbitmap::set_begin_end_index(int beg, int end)
{
	COLT_TRACE("coltbitmap::set_begin_end_index(int beg, int end)")
	coltthru::set_begin_end_index(beg, end);

	iterate_count = beg;
	min_value = beg;
	max_value = end;
}

char **coltbitmap::fields(int rec_num)
{
	return operand->fields(rec_num);
}

void coltbitmap::process_all()
{
	COLT_TRACE("coltbitmap::process_all()")
	if(!out_object)
		return;

	if(!out_object->preprocess())
		return;

	try {
		for(int index_number=min_value; index_number<=max_value; index_number++) {
			if(is_set(index_number) || all_bits_set())
				out_object->process(index_number);
		}
	} catch (colt_exception *e) {

	}

	out_object->postprocess();
}

int coltbitmap::preprocess()
{
	COLT_TRACE("coltbitmap::preprocess()")
	if(preloaded())
		return !out_object || out_object->preprocess();

//	if(!map) {
//		allocate(min_value, max_value);
//	}

	return !out_object || out_object->preprocess();
}

int coltbitmap::process(int rec_num)
{
	COLT_TRACE("coltbitmap::process(int rec_num)")
	if(!preloaded()) {
		if(!map)
			allocate(rec_num, operand->num_lines());
		set(rec_num);
	}
	return !out_object || out_object->process(rec_num);
}

void coltbitmap::postprocess()
{
	COLT_TRACE("coltbitmap::postprocess()")
	if(preloaded())
		return;

	min_value = 0;
	for(int i=0; i<num_elements; i++) {
		if(map[i] == 0)
			min_value += COLT_MAP_BITS_IN_ELEM;
		else {
			for(int j=0; j<COLT_MAP_BITS_IN_ELEM; j++)
				if(is_set(min_value+j)) {
					min_value += j;
					break;
				}
			break;
		}
	}

	max_value = num_elements * COLT_MAP_BITS_IN_ELEM;
	for(int i=num_elements-1; i>=0; i--) {
		if(map[i] == 0)
			max_value -= COLT_MAP_BITS_IN_ELEM;
		else {
			for(int j=0; j<COLT_MAP_BITS_IN_ELEM; j++)
				if(is_set(max_value-j)) {
					max_value -= j;
					break;
				}
			break;
		}
	}

	coltthru::postprocess();
//	show();
}
