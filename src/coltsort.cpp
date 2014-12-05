/*
 * coltsort.cpp
 *
 *  Created on: May 7, 2014
 *      Author: paulcarmody
 */
#include "colt_headers.h"

//
//
//

int colt_sort_column;
colt_base *sort_obj;
colt_base *tmp_file;

int sort_direction;

int datattype_sort_by_col(const void *l, const void *k)
{
	int *i = (int *)l, *j = (int *)k;
	colt_datatype *left = sort_obj->cells(*i)[colt_sort_column];
	colt_datatype *rite = sort_obj->cells(*j)[colt_sort_column];

	if(sort_direction == 1)
		return left->compare(*rite);

	return rite->compare(*left);
}

int	char_sort_by_col(const void *l, const void *k)
{
	int *i = (int *)l, *j = (int *)k;
	char *left = sort_obj->fields(*i)[colt_sort_column];
	char *rite = sort_obj->fields(*j)[colt_sort_column];
	if(sort_direction == 1)
		return strcmp(left, rite);
//		return strcmp(sort_obj->field_val(*i, colt_sort_column), sort_obj->field_val(*j, colt_sort_column));
	else
		return strcmp(rite, left);
//		return strcmp(sort_obj->field_val(*j, colt_sort_column), sort_obj->field_val(*i, colt_sort_column));
}

int	int_sort_by_col(const void *l, const void *k)
{
	int *i = (int *)l, *j = (int *)k;
	char *left = sort_obj->fields(*i)[colt_sort_column];
	char *rite = sort_obj->fields(*j)[colt_sort_column];

	if(sort_direction == 1)
		return atoi(left) - atoi(rite);
//		return atoi(sort_obj->field_val(*i, colt_sort_column)) - atoi(sort_obj->field_val(*j, colt_sort_column));
	else
		return atoi(rite) - atoi(left);
//		return atoi(sort_obj->field_val(*i, colt_sort_column)) - atoi(sort_obj->field_val(*j, colt_sort_column));
}

//
//  colt_sort declarations
//

colt_sort::colt_sort(char *source):
	coltthru(source),
	column_number(0),
	numeric_sort(0),
	ascending(0)
{
	i_am = colt_class_sort;
}

colt_sort::colt_sort(colt_base &b, int col_num, int numeric, int asc):
	coltthru(b),
	column_number(col_num),
	numeric_sort(numeric),
	ascending(asc)
{
	i_am = colt_class_sort;
}

colt_sort::colt_sort(colt_base &b, char *col_name, int numeric, int asc):
	coltthru(b),
	column_number(-1),
	numeric_sort(numeric),
	ascending(asc)
{
	i_am = colt_class_sort;
	for(int j=0; j<coltthru::num_cols(); j++) {
		char *head = coltthru::col_header( j );
		if(strcmp(col_name, head ) == 0 ) {
			column_number = j;
			break;
		}
	}

	if(column_number == -1) {
		cerr << "Unknown sort key '" << col_name << "'\n";
		exit(-1);
	}
}


colt_sort::colt_sort(colt_base &b, char *file, char *col_name, int numeric, int asc):
		coltthru(b, file),
		column_number(-1),
		numeric_sort(numeric),
		ascending(asc)
{
	i_am = colt_class_sort;
	if(!col_name)
		return;

	for(int j=0; j<coltthru::num_cols(); j++) {
		char *head = coltthru::col_header( j );
		if(strcmp(col_name, head ) == 0 ) {
			column_number = j;
			break;
		}
	}
}

colt_sort::~colt_sort() {
	// TODO Auto-generated destructor stub
}

colt_base *colt_sort::copy(colt_base *op)
{
	COLT_TRACE("*colt_sort::copy(colt_base *op)")
	coltthru *retval = new colt_sort(*op, column_number, numeric_sort, ascending);
	int size = sizeof(int) * index_count;
	retval->index_list = (int *) malloc(size);
	memcpy((void *) retval->index_list, (void *) index_list, size);
	retval->index_count = index_count;
	retval->index_record_size = index_record_size;
	return retval;
}

int colt_sort::serialize_id_num()
{
	COLT_TRACE("colt_sort::serialize_id_num()")
	return colt_sort_version;
}

int colt_sort::write_config(FILE *out)
{
	COLT_TRACE("colt_sort::write_config(FILE *out)")
	colt_sort_identifier ident;
	ident.id = serialize_id_num();
	ident.index_count = index_count;
	ident.column_number = column_number;
	ident.numeric_sort = numeric_sort;
	ident.ascending = ascending;
	strcpy(ident.file_name, colt_operator::source_file_name());

	fwrite(&ident, 1, sizeof(ident), out);
	return sizeof(ident);

}

int *colt_sort::read_config(int *in)
{
	COLT_TRACE("*colt_sort::read_config(int *in)")
    colt_sort_identifier ident;

    memcpy((void *) &ident, (void *) index_list, sizeof(ident));

    index_count = ident.index_count;
	column_number = ident.column_number;
	numeric_sort = ident.numeric_sort;
	ascending = ident.ascending;

	if(match(ident.file_name, "thru$"))
		operand = colt_load_thru(ident.file_name);
	else{
		operand = new colt_csv(ident.file_name, 1);
		((colt_csv *) operand)->open_and_load();
	}

    operand->out_object = this;

    return in + sizeof(ident)/sizeof(int);

}

int colt_sort::show_status(char *base_ptr, int indent)
{
	COLT_TRACE("colt_sort::show_status(char *base_ptr, int indent)")
	colt_sort_identifier ident;

	 memcpy((void *) &ident, (void *) base_ptr, sizeof(ident));

	 char indent_str[60];
	 for(int i=0; i<60; i++) indent_str[i] = ' ';
	 indent_str[indent*2] = '\0';
	 char *sort_order, *sort_type;

	 if(numeric_sort)
		 sort_type = "numerically";
	 else
		 sort_type = "alphabetically";

	 if(ascending)
		 sort_order = "ascending";
	 else
		 sort_order = "descending";

	 if(ident.id == colt_keyspace_version)
		 std::cout << indent_str << "keys: \t" << file_name << "\n";
	 else if(ident.id == colt_cthru_version)
		 std::cout << indent_str << "cthru: \t" << file_name << "\n";
	 else
		 std::cout << indent_str << "sort_name: \t" << file_name << "\n";

	 std::cout << indent_str << "num_lines: \t" << ident.index_count << "\n";
	 std::cout << indent_str << "col_num: \t" << column_number << ": " << col_header(column_number) << "\n";
	 std::cout << indent_str << "order: \t" << sort_type << "\n";
	 std::cout << indent_str << "direction: \t" << sort_order << "\n";

	 if(match(ident.file_name, "\.thru")) {
		 std::cout << indent_str << "data_source:\n";
		 colt_load_thru(ident.file_name, indent+1);
	 } else
		 std::cout << indent_str << "data_source:" << ident.file_name << "\n";

	 return 1;
}

void colt_sort::initialize()
{
	COLT_TRACE("colt_sort::initialize()")
	fill_sequential();
//	colt_sort_column = column_number;
//	sort_obj = operand;
//	tmp_file = operand;
//	if(numeric_sort)
//		qsort(index_list, index_count, index_record_size, &int_sort_by_col);
//	else
//		qsort(index_list, index_count, index_record_size, &char_sort_by_col);
}

//
// search fields
//

int search_low = 0;
int search_high = LONG_MAX;
char *search_low_str = "x";
char *search_high_str = "x";
int search_low_str_len = 0;
int search_high_str_len = 0;
int result_low = 0;
int result_high = LONG_MAX;

int colt_sort::search(char *min, char *max, colt_range *retobj)
{
	COLT_TRACE("colt_sort::search(char *min, char *max, colt_range *retobj)")
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

	if(retobj) {
		retobj->set_low(result_low);
		retobj->set_high(result_high);
	}

	return result_low || result_high;
}

void colt_sort::search_lowest_low(int low, int high)
{
	COLT_TRACE("colt_sort::search_lowest_low(int low, int high)")
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

void colt_sort::search_highest_high(int low, int high)
{
	COLT_TRACE("colt_sort::search_highest_high(int low, int high)")
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

void colt_sort::postprocess()
{
	COLT_TRACE("colt_sort::postprocess()")
	if(!preload)
		perform_sort();

	coltthru::postprocess();
}

void colt_sort::perform_sort()
{
	COLT_TRACE("colt_sort::perform_sort()")
	colt_sort_column = column_number;
	sort_obj = operand;
	sort_direction = ascending;
	tmp_file = operand;

	if(numeric_sort)
		qsort(index_list, index_count, index_record_size, &int_sort_by_col);
	else
		qsort(index_list, index_count, index_record_size, &char_sort_by_col);
}
