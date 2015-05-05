/*
 * coltqueuethru.cpp
 *
 *  Created on: May 1, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltqueuethru.h"

colt_queuethru::colt_queuethru(char *file_name, char col_sep, char eol_sep, char q_char):
	colt_csv(file_name, col_sep, eol_sep, q_char),
	thru_list(),
	queue_index(0)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_queuethru;

}

colt_queuethru::~colt_queuethru() {
	// TODO Auto-generated destructor stub
}

int colt_queuethru::append_thru(colt_base *thru)
{
	FILE *append = fopen(file_name, "a");

	if(thru->is_a(colt_class_queuethru))
		fprintf(append, "%s\n", thru->source_file_name());
	else {
		char tmp[COLT_MAX_STRING_SIZE];
		coltthru *mythru = (coltthru*) thru;
		mythru->to_string(tmp);
		fprintf(append, "%s\n", tmp);
	}

	fclose(append);
}

int colt_queuethru::open_and_load()
{
	COLT_TRACE("cotl_queuethru::open_load()")
	colt_csv::open_and_load();

	for(int i=0; i<lines.size(); i++) {
		char *rec = record(i);
		colt_datatype tmp(COLT_DT_SOURCE);
		tmp.set_value(rec);
		thru_list.push_back((colt_base *)tmp.get_value());
	}
}

int	colt_queuethru::find_sep_chars(int set_sep_chars)
{
	COLT_TRACE("colt_queuethru:: find_sep_chars(int set_sep_chars)")
	return 1;
}

int  colt_queuethru::load_headers()
{
	COLT_TRACE("colt_queuethru::load_headers()")

	col_count = 1;
	headers = (char **) malloc(sizeof(char*)*col_count);
	headers[0] = base_ptr;

	cell_objects = (colt_datatype **) malloc(sizeof(colt_datatype*)*col_count);
	cell_objects[0] = new colt_datatype(COLT_DT_SOURCE);

	return col_count;
}

int colt_queuethru::get_next_index(int curr_index, int size, char *buff)
{
	COLT_TRACE("colt_queuethru::get_next_index(int curr_index, int size, char *buff)")
    while(curr_index < size && buff[curr_index] != '\n'){
        curr_index++;
    }

    if(buff[curr_index] == '\n' && curr_index+1 < size) {
    	return curr_index + 1;
    }

    return 0;
}
//
//int colt_queuethru::get_next_row()
//{
//	COLT_TRACE("colt_queuethru::get_next_row()")
//
//	int tmp_index = queue_index;
//
//	int i;
//	for(i=0; i < thru_list.size() && tmp_index > thru_list[i]->num_lines(); i++)
//		tmp_index -= thru_list[e]->num_lines();
////	if(preload) {
////		++line_counter;
////		if(line_counter >= num_lines())
////			return -1;
////		return line_counter;
////	}
////
////	if((next_index = get_next_index(curr_index, file_size, base_ptr)) > 0) {
////		lines.push_back(next_index);
////		curr_index = next_index;
////	}
////
////	++line_counter;
////
////	if(line_counter >= num_lines())
////		return -1;
////
////	return line_counter;
//}

//char *colt_queuethru::record(int line_num);
//char *colt_queuethru::col_header(int n);
//char **colt_queuethru::col_headers();
//int colt_queuethru::num_lines();
//int colt_queuethru::num_cols();
//int colt_queuethru::max_size();


char **colt_queuethru::fields(int rec_num)
{
	COLT_TRACE("**colt_queuethru::fields(int rec_num)")
//	if(fields_retval)
//		free(fields_retval);
	_trace.start() << " " << rec_num << ":" << num_lines() << ":" << file_name << "!\n";

	if(rec_num > num_lines())
		rec_num = num_lines();
	_trace.start() << " " << rec_num << "\n";

	int i = 0;
	fields_retval[0] = record(rec_num);
	int number_of_cols = num_cols();
	for(char *x = fields_retval[0]; i<number_of_cols; x++)
		if(!*x) {
			fields_retval[++i] = x+1;
		}

	return fields_retval;
}


//int	colt_queuethru::meta_num_cols();
//char *colt_queuethru::meta_col_header(int n);
//char **colt_queuethru::meta_col_headers();
//char **colt_queuethru::meta_fields(int rec_num);
//colt_datatype **colt_queuethru::meta_cells(int rec_num);
//int	colt_queuethru::get_meta_row(int rec_num);
