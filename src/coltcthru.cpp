/*
 * coltcthru.cpp
 *
 *  Created on: May 19, 2014
 *      Author: paulcarmody
 */
#include "colt_headers.h"

colt_cthru::colt_cthru(char *file):
	colt_sort(file),
	node_list(NULL)
{
	i_am = colt_class_cthru;
	index_record_size = sizeof(colt_record);
}

colt_cthru::colt_cthru(colt_base &in, int col_num, int numeric, int asc):
	colt_sort(in, col_num, numeric),
	node_list(NULL)
{
	i_am = colt_class_cthru;
	// TODO Auto-generated constructor stub
	index_record_size = sizeof(colt_record);
//	initialize();
}

colt_cthru::colt_cthru(colt_base &b, char *col_name, int numeric, int asc):
		colt_sort(b, col_name, numeric, asc),
		node_list(NULL)
{
	i_am = colt_class_cthru;
	index_record_size = sizeof(colt_record);
}

colt_cthru::colt_cthru(colt_base &b, char *file, char *col_name, int numeric, int asc):
		colt_sort(b, file, col_name, numeric, asc),
		node_list(NULL)
{
	i_am = colt_class_cthru;
	index_record_size = sizeof(colt_record);
}

colt_cthru::~colt_cthru() {
	// TODO Auto-generated destructor stub
}

int colt_cthru::serialize_id_num()
{
	return colt_cthru_version;
}

void colt_cthru::initialize()
{
	colt_sort::initialize();
	int lines = num_lines();

	node_list = (colt_record *) malloc(lines * index_record_size);

	for(int i=0; i<num_lines(); i++)
		node_list[i].record_number = index_list[i];
//	for(colt_counter *i=get_row_counter(); !i->done(); i->next())
//		node_list[i->index()].record_number = i->index();
	init(0, lines-1);
}

void colt_cthru::push_back(int i)
{
	node_list[index_count++].record_number = i;
}

int colt_cthru::index(int i)
{
	return node_list[i].record_number;
}

colt_cthru::colt_record *colt_cthru::init(int low, int high)
{
	int center = low + (high - low)/2;
	node_list[center].min_value_below = -1;
	node_list[center].max_value_below = -1;
	node_list[center].num_nodes_below = 1;
//	node_list[center].sum_nodes_below = atoi(field_val(center, col_num()));

	if(high == low  || high > num_lines()) {
		return &node_list[center];
	}

	if(low <= center-1) {
		int center_low = low + (center-1 - low)/2;
		colt_record *lower_half = init(low, center-1);
		if(lower_half->min_value_below >= 0) {
			node_list[center].min_value_below = lower_half->min_value_below;
			node_list[center].num_nodes_below += lower_half->num_nodes_below;
		} else {
			node_list[center].min_value_below = center_low;
			node_list[center].num_nodes_below++;
		}
//		node_list[center].sum_nodes_below += lower_half->sum_nodes_below;
	}

	if(center+1 <= high) {
		int center_high = center+1 + (high - center)/2;
		colt_record *upper_half = init(center+1, high);
		if(upper_half->max_value_below >= 0) {
			node_list[center].max_value_below = upper_half->max_value_below;
			node_list[center].num_nodes_below += upper_half->num_nodes_below;
		} else {
			node_list[center].max_value_below = center_high;
			node_list[center].num_nodes_below++;
		}
//		node_list[center].sum_nodes_below += upper_half->sum_nodes_below;
	}

	return &node_list[center];
}

int colt_cthru::search(char *cond, colt_range *retobj)
{
	return 0;
}

//
// search fields
//

extern int search_low;
extern int search_high;
extern char *search_low_str;
extern char *search_high_str;
extern int search_low_str_len;
extern int search_high_str_len;
extern int result_low;
extern int result_high;

int colt_cthru::search(char *min, char *max, colt_range *retobj)
{
	if(numeric_sort) {
		search_low = atoi(min);
		if(search_low < atoi(field_val(0, col_num())) )
			search_low = atoi(field_val(0, col_num()));

		search_high = atoi(max);
		if(search_high > atoi(field_val(num_lines()-1, col_num())) )
			search_high = atoi(field_val(num_lines()-1, col_num()));

		if(search_high < search_low) {
			int tmp = search_high;
			search_high = search_low;
			search_low = tmp;
		}

	} else {
		search_low_str = min;
		search_high_str = max;
		search_low_str_len = strlen(search_low_str);
		search_high_str_len = strlen(search_high_str);
	}

	search_lowest_low(0, num_lines());
	search_highest_high(0, num_lines());

	retobj->set_low(result_low);
	retobj->set_high(result_high);

	return 1;
}

void colt_cthru::search_lowest_low(int low, int high)
{
	if(low > high) {
		result_low = low;
		return;
	}

	int center = low + (high - low)/2;

	int condition = 0;
	if(numeric_sort) {
		int value = atoi(field_val(center, col_num()));
		condition = search_low - value;
	} else
		condition = strncmp(search_low_str, field_val(center, col_num()), search_low_str_len);

	if(condition == 0) {
		int cond = 0;
		if(numeric_sort) {
			char *val = field_val(center-1, col_num());
			if(!val) {
				result_low = center;
				return;
			}
			int highest_left = atoi(val);
			cond = highest_left - search_low;
		} else
			cond = strncmp(search_low_str, field_val(center-1, col_num()), search_low_str_len);
		if(cond != 0)
//		if(highest_left != search_low)
			result_low = center;
		else if (low == center)
			result_low = center;
		else
			search_lowest_low(low, center-1);
//	} else if(search_low < value)
	} else if(condition < 0)
		search_lowest_low(low, center-1);
	else
		search_lowest_low(center+1, high);
}

void colt_cthru::search_highest_high(int low, int high)
{
	if(low > high) {
		result_high = low-1;
		return;
	}

	int center = low + (high - low)/2;
	int condition = 0;

	if(numeric_sort) {
		int value = atoi(field_val(center, col_num()));
		condition = search_high - value;
	} else
		condition = strncmp(search_high_str, field_val(center, col_num()), search_high_str_len);

	if(condition == 0) {
		int cond = 0;
		if(numeric_sort) {
			int lowest_right = atoi(field_val(center+1, col_num()));
			cond = lowest_right - search_high;
		} else
			cond = strncmp(search_high_str, field_val(center+1, col_num()), search_high_str_len);
		if(cond != 0)
			result_high = center;
//		else if(high == num_lines())
//			result_high = high;
		else
			search_highest_high(center+1, high);
	} else if(condition < 0)
		search_highest_high(low, center-1);
	else
		search_highest_high(center+1, high);
}

char *colt_cthru::record(int line_num)
{
	return operand->record(node_list[line_num].record_number);
}

char *colt_cthru::field_val(int rec_num, int col_num)
{
	return colt_sort::field_val(node_list[rec_num].record_number, col_num);
}

char **colt_cthru::fields(int rec_num)
{
	return operand->fields(node_list[rec_num].record_number);
}

colt_datatype **colt_cthru::cells(int rec_num)
{
	return operand->cells(node_list[rec_num].record_number);
}

void colt_cthru::save(char *file_name)
{
//	FILE *out = fopen(file_name, "wb");
//	fwrite(index_list, index_record_size, index_count, out);
//	fclose(out);
	int *tmp = index_list;
	index_list = (int *) node_list;
	colt_sort::save(file_name);
	index_list = (int *) tmp;
}

int *colt_cthru::read_config(int *in)
{
	int *retval = colt_sort::read_config(in);

	node_list = (colt_record *) retval;

	return retval;
}

void colt_cthru::perform_sort()
{
	if(preload)
		return;

	if(index_list)
		free(index_list);

	index_list = (int *) node_list;

	colt_sort::perform_sort();

	init(0, index_count-1);
}

int colt_cthru::preprocess()
{
	if(preload) {
		node_list = (colt_record *) index_list;
		if(out_object)
			return out_object->preprocess();
		return 1;
	}

	if(node_list)
		free(node_list);

	index_record_size = sizeof(colt_record);
	node_list = (colt_record *) malloc(max_size() * index_record_size);

	return out_object && out_object->preprocess();
}

void colt_cthru::postprocess()
{
	colt_sort::postprocess();

//	print();
//	out_object->postprocess();
}
