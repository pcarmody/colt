/*
 * coltcsv.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
#include "coltdatatype.h"
#include "coltbase.h"
#include "coltcsv.h"
#include "colttrace.h"

#define COLT_TRACE(MSG) colt_trace _trace(MSG);

int colt_index_file_exists(char *file_name)
{
	char index_file_name[100];
	strcpy(index_file_name, file_name);
	strcat(index_file_name, ".ndx");

	struct stat buffer;

	return (stat(index_file_name, &buffer) == 0);
}

colt_csv::colt_csv(char *fname, int pl)
{
	i_am = colt_class_csv;
	preload = pl;
	column_sep_char = '\0';
	end_of_line_sep_char = '\0';
	quote_char = '\0';
	strcpy(file_name, fname);
	fdescriptor = 0;
	base_ptr = NULL;
	col_count = 0;
	line_counter = 0;
	fields_retval = NULL;
//	if(colt_index_file_exists(file_name))
//		preload_data();
//	else
//		open_file(1);
}

colt_csv::colt_csv(char *fname, char col_sep, char eol_sep, char q_char)
{
	i_am = colt_class_csv;
	preload = 0;
	column_sep_char = col_sep;
	end_of_line_sep_char = eol_sep;
	quote_char = q_char;
	strcpy(file_name, fname);
	fdescriptor = 0;
	base_ptr = NULL;
	col_count = 0;
	line_counter = 0;
	fields_retval = NULL;
}

int colt_csv::open_and_load()
{
	COLT_TRACE("colt_csv::open_and_load()")
	if(colt_index_file_exists(file_name))
		preload_data();
	else
		open_file(1);
}

colt_csv::~colt_csv() {

	if (fdescriptor && base_ptr && munmap (base_ptr, size) == -1) {
		  perror ("munmap");
	}
	close(fdescriptor);

	if(fields_retval)
		free(fields_retval);

	for(int i=0; i<col_count; i++)
		delete cell_objects[i];
	free(cell_objects);
//
//	if(headers)
//		free(headers);

	char index_file_name[100];
	strcpy(index_file_name, file_name);
	strcat(index_file_name, ".ndx");

	ofstream os(index_file_name, ios::binary);
	os.write((const char *) &col_count, sizeof(int));
	os.write((const char *)&lines[0], lines.size() * sizeof(int));
	os.close();
}

colt_base *colt_csv::copy(colt_base *op)
{
	COLT_TRACE("*colt_csv::copy(colt_base *op)")
	return new colt_csv(file_name, column_sep_char, end_of_line_sep_char, quote_char);
}

char colt_csv::sep_char()
{
	COLT_TRACE("colt_csv::sep_char()")
	return column_sep_char;
}

char *colt_csv::source_file_name()
{
	COLT_TRACE("*colt_csv::source_file_name()")
	return file_name;
}

int colt_csv::num_lines()
{
	COLT_TRACE("colt_csv::num_lines()")
	return lines.size()-1;
};

int colt_csv::max_size()
{
	COLT_TRACE("colt_csv::max_size()")
	if(line_counter < 0)
		return num_lines();

	return file_size;
};

int colt_csv::get_next_index(int curr_index, int size, char *buff)
{
	COLT_TRACE("colt_csv::get_next_index(int curr_index, int size, char *buff)")
    while(curr_index < size && buff[curr_index] != '\n'){
    	if(buff[curr_index] == column_sep_char) {
    		buff[curr_index] = '\0';
    		if(buff[curr_index+1] == quote_char) {
    			curr_index += 2;
    			while(buff[curr_index] && buff[curr_index] != quote_char) {
//    				buff[curr_index-1] = buff[curr_index];
    				curr_index++;
    			}
    		}
    	}
        curr_index++;
    }

    if(buff[curr_index] == '\n') {
    	buff[curr_index] = '\0';
    	return curr_index + 1;
    }

    return 0;
}

char *colt_csv::extract_str(char *in, char sep_char, char eol_char, char q_char)
{
	COLT_TRACE("*colt_csv::extract_str(char *in, char sep_char, char eol_char, char q_char)")
	return in;
}

int colt_csv::open_file (int set_sep_chars)
{
	COLT_TRACE("colt_csv::open_file (int set_sep_chars)")
	struct stat sb;
      char *p;
      int fd;

      fd = open (file_name, O_RDWR );
      if (fd == -1) {
              perror ("open");
              return 0;
      }

      if (fstat (fd, &sb) == -1) {
              perror ("fstat");
              return 0;
      }

      if (!S_ISREG (sb.st_mode)) {
              fprintf (stderr, "%s is not a file\n", file_name);
              return 1;
      }

      p = (char *) mmap (0, sb.st_size, PROT_WRITE, MAP_SHARED, fd, 0);
      if (p == MAP_FAILED) {
              perror ("mmap");
              return 0;
      }

      base_ptr = p;
      fdescriptor = fd;
      size = sb.st_size;

      file_size = sb.st_size;

      if(!preload) {
		  find_sep_chars(set_sep_chars);
		  load_headers();

		  int curr_index = 0;
		  int next_index;

		  while((next_index = get_next_index(curr_index, file_size, base_ptr)) > 0) {
			  lines.push_back(next_index);
			  curr_index = next_index;
		  }

		  // save the index file

		  char index_file_name[100];
		  strcpy(index_file_name, file_name);
		  strcat(index_file_name, ".ndx");

		  ofstream os(index_file_name, ios::binary);
		  os.write((const char *) &col_count, sizeof(int));
		  os.write((const char *)&lines[0], lines.size() * sizeof(int));
		  os.close();
      }

      return 1;
}

int	colt_csv:: find_sep_chars(int set_sep_chars)
{
	COLT_TRACE("colt_csv:: find_sep_chars(int set_sep_chars)")
    if(!set_sep_chars)
  	  return 1;

    end_of_line_sep_char = '\n';


	  int test[127];
	  for(int i=0; i<127; i++) test[i] = '\0';
	  column_sep_char = 0;
	  int line_count = 3;
	  for(char *x=base_ptr; *x; x++) {
		  if(*x == '\n')
			  if(!--line_count)
				  break;
		  if(*x == '\t'
		  || (*x >= 33 && *x <= 47)
		  || (*x >= 58 && *x <= 64)
		  || (*x >= 91 && *x <= 94)
		  || *x == 96
		  || *x == 124) {
			  ++test[*x];
			  if(*x == '"' || *x == '\'' || *x == '`' || *x == '_') {
				  if(test[*x] > test[quote_char])
					  quote_char = *x;
			  } else
				  if(test[*x] > test[column_sep_char])
					  column_sep_char = *x;
		  }

	  }

	  return 1;
}

int  colt_csv::load_headers()
{
	COLT_TRACE("colt_csv::load_headers()")
	char *b = base_ptr;
	char *tmp_header = b;
	char *tmp_array[COLT_MAX_NUM_COLS];

	for(char *x = base_ptr; *x != end_of_line_sep_char; x++) {
		if(col_count > COLT_MAX_NUM_COLS)
			continue;
		if(*x == column_sep_char) {
			*b++ = '\0';
			tmp_array[col_count] = tmp_header;
			col_count++;
			tmp_header = b;
		} else
			*b++ = *x;
	}

	*b = '\0';
	tmp_array[col_count] = tmp_header;
	col_count++;

	headers = (char **) malloc(sizeof(char*)*col_count);
	memcpy(headers, tmp_array, col_count*sizeof(char*));

	cell_objects == (colt_datatype **) malloc(sizeof(colt_datatype*)*col_count);
	for(int i=0; i<col_count; i++)
		cell_objects[i] = new colt_datatype;
}


int colt_csv::preload_data()
{
	COLT_TRACE("colt_csv::preload_data()")
	char index_file_name[100];
	strcpy(index_file_name, file_name);
	strcat(index_file_name, ".ndx");

	int fd;
	struct stat sb;

	fd = open (index_file_name, O_RDONLY);
	if (fd == -1) {
		  perror ("open");
		  return 0;
	}
	if (fstat (fd, &sb) == -1) {
		perror ("fstat");
		return 0;
	}

	int *p = (int *) mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

	col_count = *p;

	for(int i=1; i<sb.st_size/sizeof(int); i++) {
		lines.push_back(p[i]);
	}

	close(fd);

	preload = 1;
	open_file();

	char *tmp_array[COLT_MAX_NUM_COLS];
	int i=1;
	tmp_array[0] = base_ptr;

	for(char *x = base_ptr; i < col_count; x++) {
		if(*x == '\0')
			tmp_array[i++] = x+1;
	}

	headers = (char **) malloc(sizeof(char*)*col_count);
	memcpy(headers, tmp_array, col_count*sizeof(char*));

	cell_objects = (colt_datatype **) malloc(sizeof(colt_datatype*)*col_count);
	for(int i=0; i<col_count; i++) {
		colt_datatype *tmp = new colt_datatype;
		cell_objects[i] = tmp;
	}
	preload = 1;

	return 1;
}

int colt_csv::show_status(int indent_level)
{
	COLT_TRACE("colt_csv::show_status(int indent_level)")
	return 0;
}

int colt_csv::num_cols()
{
	COLT_TRACE("colt_csv::num_cols()")
	return col_count;
}

char *colt_csv::record(int line_num) 
{
	COLT_TRACE("*colt_csv::record(int line_num) ")
	if(line_num >= lines.size())
		line_num = lines.size()-1;
	return base_ptr + lines[line_num];
}

char *colt_csv::col_header(int n)
{
	COLT_TRACE("*colt_csv::col_header(int n)")
	return headers[n];
}

char colt_buffer[1000];

char *colt_csv::field_val(int rec_num, int col_num) 
{
	COLT_TRACE("*colt_csv::field_val(int rec_num, int col_num) ")
	char *rec = record(rec_num);
	int cnt = 0;

	while(cnt < col_num) {
		while(*rec && cnt < col_num) rec++;
		cnt++;
		rec++;
	}
	return rec;
}

char **colt_csv::fields(int rec_num)
{
	COLT_TRACE("**colt_csv::fields(int rec_num)")
//	if(fields_retval)
//		free(fields_retval);
	if(rec_num > num_lines())
		rec_num = num_lines();

	int i = 0;
	fields_retval[0] = record(rec_num);
	int number_of_cols = num_cols();
	for(char *x = fields_retval[0]; i<number_of_cols; x++)
		if(!*x) {
			fields_retval[++i] = x+1;
		}

	return fields_retval;
}

void colt_csv::set_coltype(int num, colt_datatype *x)
{
	COLT_TRACE("colt_csv::set_coltype(int num, colt_datatype *x)")
	if(!cell_objects) {
		cell_objects = (colt_datatype **) malloc( sizeof(colt_datatype *) * num_cols());
		for(int i=0; i<num_cols(); i++) cell_objects[i] = NULL;
	}

	if(num >= num_cols())
		throw "Attempted to set the data type of a column that doesn't exist.\n";

	cell_objects[num] = x;
}

void colt_csv::set_datatype(int i, int type)
{
	if(cell_objects[i]) {
		if(cell_objects[i]->type == type)
			return;
		delete cell_objects[i];
	}

	if(type == COLT_DATATYPE)
		cell_objects[i] = new colt_datatype;
	else if(type == COLT_DT_INTEGER)
		cell_objects[i] = new colt_integer;
}

colt_datatype **colt_csv::cells(int rec_num)
{
	COLT_TRACE("**colt_csv::cells(int rec_num)")
	if(rec_num > num_lines())
		rec_num = num_lines();

	if(!cell_objects)
		return cell_objects;

	char **f = fields(rec_num);

	int number_of_cols = num_cols();
	for(int i=0; i< number_of_cols; i++)
		cell_objects[i]->set_buffer(f[i]);
//	int i = 1;
//	char *start = record(rec_num);
//	cell_objects[0]->set_buffer(start);
//	int number_of_cols = num_cols();
//
//	for(char *x = start; i<=number_of_cols; x++)
//		if(!*x)
//			cell_objects[++i]->set_buffer(x+1);

	_trace.start() << " cells filled\n";

	return cell_objects;
}

bool colt_csv::sort_func(int i, int j)
{
	COLT_TRACE("colt_csv::sort_func(int i, int j)")
	char a[80];
	char b[80];
	strcpy(a, field_val(i,6));
	strcpy(b, field_val(j,6));
	return strcmp(a,b) < 0;
}

int colt_csv::get_next_row()
{
	COLT_TRACE("colt_csv::get_next_row()")
	if(preload) {
		++line_counter;
		if(line_counter >= num_lines())
			return -1;
		return line_counter;
	}

	if((next_index = get_next_index(curr_index, file_size, base_ptr)) > 0) {
		lines.push_back(next_index);
		curr_index = next_index;
	}

	++line_counter;

	if(line_counter >= num_lines())
		return -1;

	return line_counter;
}

int colt_csv::preprocess()
{
	COLT_TRACE("colt_csv::preprocess()")
	line_counter=-1;

	if(!fields_retval)
		fields_retval = (char **) malloc(sizeof(char *) * (num_cols() + 1));

	if(preload)
		return colt_base::preprocess();

	curr_index = 0;

	if((next_index = get_next_index(curr_index, file_size, base_ptr)) > 0) {
		lines.push_back(next_index);
		curr_index = next_index;
	}

	return colt_base::preprocess();
}
