/*
 * coltsequentialcsv.cpp
 *
 *  Created on: Oct 19, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltsequentialcsv.h"

#include <sys/stat.h>

colt_sequential_csv::colt_sequential_csv(char *file_name, char col_sep, char eol_sep, char q_char):
	colt_zsv(file_name, col_sep, eol_sep, q_char)
{
	i_am = colt_class_sequential_csv;
	for(int i=0; i<COLT_MAX_STRING_SIZE; i++)
		sequential_file_buffer[0] = '\0';
}

colt_sequential_csv::colt_sequential_csv(char *file_name, int pl):
	colt_zsv(file_name, pl)
{
	i_am = colt_class_sequential_csv;
	for(int i=0; i<COLT_MAX_STRING_SIZE; i++)
		sequential_file_buffer[0] = '\0';
	preload = 0;
}

colt_sequential_csv::~colt_sequential_csv()
{
	if(headers)
		free(headers);
}

int colt_sequential_csv::open_file(int set_sep_chars)
{
//      fd = open (file_name, O_RDWR );

	if(file_name[0]) {
		fdesc_ptr = fopen(file_name, "r");
		struct stat st;
		stat(file_name, &st);
		file_size = st.st_size;
	} else {
		fdesc_ptr = stdin;
		file_size = -1;
	}

	if (!fdesc_ptr) {
		fprintf(stderr, "Cannot open '%s'.\n", file_name);
		return 0;
	}

    base_ptr = sequential_file_buffer;

	find_sep_chars(set_sep_chars);

//      get_next_row();
   	fgets(base_ptr, COLT_MAX_STRING_SIZE, fdesc_ptr);
    find_sep_chars(1);

    load_headers();

    return 1;
}

int  colt_sequential_csv::load_headers()
{
	char tmp_header[COLT_MAX_STRING_SIZE];
	char *b = tmp_header;
	char *tmp_array[COLT_MAX_NUM_COLS];
	col_count = 0;

	for(char *x = base_ptr; *x != end_of_line_sep_char; x++) {
		if(col_count > COLT_MAX_NUM_COLS)
			continue;
		if(*x == column_sep_char) {
			*b = '\0';
			tmp_array[col_count] = (char *) malloc(strlen(tmp_header)+1);
			strcpy(tmp_array[col_count], tmp_header);
			col_count++;
			b = tmp_header;
		} else
			*b++ = *x;
	}

	*b = '\0';

	tmp_array[col_count] = (char *) malloc(strlen(tmp_header)+1);
	strcpy(tmp_array[col_count], tmp_header);
	col_count++;

	int headers_size = sizeof(char*)*col_count;
	headers = (char **) malloc(headers_size);
	memcpy(headers, tmp_array, headers_size);
}


char *colt_sequential_csv::record(int line_num)
{
	return base_ptr;
}

int colt_sequential_csv::num_lines()
{
	return line_counter+1;
}

int colt_sequential_csv::max_size()
{
	return num_lines();
}

int colt_sequential_csv::get_next_row()
{
    base_ptr = sequential_file_buffer;
	if(fgets(base_ptr, COLT_MAX_STRING_SIZE, fdesc_ptr)) {

		get_next_index(0, COLT_MAX_STRING_SIZE, base_ptr);

		++line_counter;

		return line_counter;
	}

	return -1;
}

int colt_sequential_csv::preprocess()
{
	if(!fields_retval)
		fields_retval = (char **) malloc(sizeof(char *) * (num_cols() + 1));

	return colt_zsv::preprocess();
}

