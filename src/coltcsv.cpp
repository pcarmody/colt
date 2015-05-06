/*
 * coltcsv.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltcsv.h"

colt_csv::colt_csv(char *file_name, char col_sep, char eol_sep, char q_char):
	colt_zsv(file_name, col_sep, eol_sep, q_char)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_csv;
}

colt_csv::~colt_csv() {
	// TODO Auto-generated destructor stub
}

int colt_csv::append_thru(colt_base *thru)
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

int	colt_csv::find_sep_chars(int set_sep_chars)
{
	COLT_TRACE("colt_csv:: find_sep_chars(int set_sep_chars)")
	return 1;
}

int  colt_csv::load_headers()
{
	COLT_TRACE("colt_csv::load_headers()")

	col_count = 0;
	headers = (char **) malloc(sizeof(char*)*col_count);
	char *b = base_ptr;
	char tmp_head[COLT_MAX_STRING_SIZE];
	char *a = tmp_head;

	while(*b != end_of_line_sep_char) {
		if(*b == column_sep_char) {
			*a = '\0';
			headers[col_count] = new char[strlen(tmp_head) +1 ];
			strcpy(headers[col_count], tmp_head);
			col_count++;
			a = tmp_head;
		}
		*a++ = *b++;
	}
//	headers[0] = base_ptr;

	*a = '\0';
	headers[col_count] = new char[strlen(tmp_head) +1 ];
	_trace.start() << "headers[0] " << headers[0];
	strcpy(headers[col_count], tmp_head);
	col_count++;

//	cell_objects = (colt_datatype **) malloc(sizeof(colt_datatype*)*col_count);
//	cell_objects[0] = new colt_datatype(COLT_DT_SOURCE);

	return col_count;
}

int colt_csv::get_next_index(int curr_index, int size, char *buff)
{
	COLT_TRACE("colt_csv::get_next_index(int curr_index, int size, char *buff)")
    while(curr_index < size && buff[curr_index] != '\n'){
        curr_index++;
    }

    if(buff[curr_index] == '\n' && curr_index+1 < size) {
    	return curr_index + 1;
    }

    return 0;
}

char **colt_csv::fields(int rec_num)
{
	COLT_TRACE("**colt_csv::fields(int rec_num)")

	int num_col = num_cols()+1;
	for(int i=0; i<num_col; i++)
		if(fields_retval[i])
			delete fields_retval[i];

	char *b = record(rec_num+1);
	char tmp_field[COLT_MAX_STRING_SIZE];
	char *a = tmp_field;
	int col = 0;

	while(*b != end_of_line_sep_char) {
		if(*b == column_sep_char) {
			*a = '\0';
			fields_retval[col] = new char[strlen(tmp_field) +1 ];
			strcpy(fields_retval[col], tmp_field);
			col++;
			a = tmp_field;
		}
		*a++ = *b++;
	}
//	headers[0] = base_ptr;

	*a = '\0';
	fields_retval[col] = new char[strlen(tmp_field) +1 ];
	strcpy(fields_retval[col], tmp_field);
	col++;

	return fields_retval;
}
