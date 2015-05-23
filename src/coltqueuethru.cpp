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
	meta_headers(NULL)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_queuethru;
	meta_file_type = "pthru";
}

colt_queuethru::~colt_queuethru() {
	// TODO Auto-generated destructor stub
	for(int i=0; i<thru_list.size(); i++)
		if(thru_list[i])
			delete thru_list[i];
}

int colt_queuethru::append_thru(colt_base *thru)
{
	COLT_TRACE("colt_queuethru::append_thru(colt_base *thru)")
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

char *colt_queuethru::source_file_name()
{
	if(thru_list.size() > 0)
		return thru_list[0]->thru->source_file_name();

	return colt_csv::source_file_name();
}

extern int colt_index_file_exists(char *file_name);

int colt_queuethru::open_and_load()
{
	COLT_TRACE("colt_queuethru::open_and_load()")
	if(colt_index_file_exists(file_name)) {
		char index_file_name[COLT_MAX_STRING_SIZE];
		sprintf(index_file_name, "%s.ndx", file_name);
		remove(index_file_name);
	}

	int accumulator = 0;
	int retval = colt_csv::open_and_load();
	int lines = num_lines();

	for(int i=0; i<lines; i++) {
		colt_datatype tmp(COLT_DT_SOURCE);
		tmp.parse_thru(colt_csv::fields(i)[0]);
		colt_qthru_thru_struct *thru_struct = new colt_qthru_thru_struct;
		thru_struct->thru = (colt_base *) tmp.get_value();
		thru_struct->sum = accumulator;
		thru_list.push_back(thru_struct);
		accumulator += thru_struct->thru->num_lines();
	}

	meta_col_headers();	// does nothing but initial*ze the meta_col_headers
	meta_fields(0);
//	meta_cells(0);

	return retval;
}

int colt_queuethru::num_lines()
{
	COLT_TRACE("colt_queuethru::num_lines()")
	if(thru_list.size() == 0)
		return colt_csv::num_lines();

	colt_qthru_thru_struct *last = thru_list[thru_list.size()-1];
	return last->sum + last->thru->num_lines();
}

int colt_queuethru::num_cols()
{
	if(thru_list.size() == 0)
		return colt_csv::num_cols();

	return thru_list[0]->thru->num_cols();
}

char **colt_queuethru::col_headers()
{
	COLT_TRACE("colt_queuethru::col_headers()")
	return thru_list[0]->thru->col_headers();
}

char **colt_queuethru::fields(int rec_num)
{
	COLT_TRACE("colt_queuethru::fields(int rec_num)")
	int i;
	int size = thru_list.size();
	int thru_rec_num = rec_num;

	for(i=0; i<size && thru_rec_num > thru_list[i]->thru->num_lines(); i++)
		thru_rec_num -= thru_list[i]->thru->num_lines();

	if(thru_list[i]->thru->num_lines() == thru_rec_num) {
		i++;
		thru_rec_num = 0;
	}

//	cout << "qqq " << rec_num << ":" << i << ":" << thru_rec_num << ":" << thru_list[i]->thru->num_lines() << "\n";
	return thru_list[i]->thru->fields(thru_rec_num);
}

colt_datatype **colt_queuethru::cells(int rec_num)
{
	COLT_TRACE("colt_queuethru::cells(int rec_num)")
	int i;
	int size = thru_list.size();
	int thru_rec_num = rec_num;

	for(i=0; i<size && thru_rec_num > thru_list[i]->thru->num_lines(); i++)
		thru_rec_num -= thru_list[i]->thru->num_lines();

	if(thru_list[i]->thru->num_lines() == thru_rec_num) {
		i++;
		thru_rec_num = 0;
	}

//	cout << "qqq " << rec_num << ":" << i << ":" << thru_rec_num << ":" << thru_list[i]->thru->num_lines() << "\n";
	return thru_list[i]->thru->cells(thru_rec_num);
}

colt_nested_cells *colt_queuethru::nested_cells(int rec_num)
{

	COLT_TRACE("colt_queuethru::nested_cells(int rec_num)")
	int i;
	int size = thru_list.size();
	int thru_rec_num = rec_num;

	for(i=0; i<size && thru_rec_num > thru_list[i]->thru->num_lines(); i++)
		thru_rec_num -= thru_list[i]->thru->num_lines();

	if(thru_list[i]->thru->num_lines() == thru_rec_num) {
		i++;
		thru_rec_num = 0;
	}

	if(!nested)
		nested = new colt_nested_cells(NULL, 0, NULL, source_file_name());

	nested->next = thru_list[i]->thru->nested_cells(thru_rec_num);
//	nested->cells = colt_csv::fields(i);

	return nested;
}


int	colt_queuethru::meta_num_cols()
{
	int retval = colt_csv::meta_num_cols() + colt_csv::num_cols();
//	cout << "qqq " << retval << "\n";
	return retval;
}

char *colt_queuethru::meta_col_header(int n)
{
	int cols = colt_csv::meta_num_cols();
	if(n > cols)
		return colt_csv::meta_col_header(n);

	return colt_csv::col_header(n-cols);
}

char **colt_queuethru::meta_col_headers()
{
	if(!meta_headers) {
		int old_size = colt_csv::meta_num_cols();
		meta_headers = (char **) malloc( (old_size + colt_csv::num_cols()) * sizeof(char *));
		char **old_headers = colt_csv::meta_col_headers();
		for(int i=0; i<old_size; i++)
			meta_headers[i] = old_headers[i];
		old_headers = colt_csv::col_headers();
		for(int j=0; j<colt_csv::num_cols(); j++)
			meta_headers[old_size+j] = old_headers[j];
	}
	return meta_headers;
}

char **colt_queuethru::meta_fields(int rec_num)
{
	int old_size = colt_csv::meta_num_cols();

	if(!metadata_fields) {
		metadata_fields = (char **) malloc( (old_size + colt_csv::num_cols()) * sizeof(colt_datatype *));
		char **old_fields = colt_csv::meta_fields(rec_num);
		for(int i=0; i<old_size; i++)
			metadata_fields[i] = old_fields[i];
		metadata_fields[7] = "qthru";
	}

//	sprintf(metadata_fields[2], "%d", thru_list[0]->thru->num_cols());

	char **old_fields = colt_csv::fields(rec_num);
	for(int j=0; j<colt_csv::num_cols(); j++)
		metadata_fields[old_size+j] = old_fields[j];

	return metadata_fields;
}

colt_datatype **colt_queuethru::meta_cells(int rec_num)
{
	int old_size = colt_csv::meta_num_cols();

	if(!metadata_cells) {
		metadata_cells = (colt_datatype **) malloc( (old_size + colt_csv::num_cols()) * sizeof(colt_datatype *));
		colt_datatype **old_cells = colt_csv::meta_cells(rec_num);
		for(int i=0; i<old_size; i++)
			metadata_cells[i] = old_cells[i];
	}

	colt_datatype **old_cells = colt_csv::cells(rec_num);
	for(int j=0; j<colt_csv::num_cols(); j++)
		metadata_cells[old_size+j] = old_cells[j];

	return metadata_cells;
}

int	colt_queuethru::get_meta_row(int rec_num)
{
	if(rec_num+1 < colt_csv::num_lines())
		return rec_num+1;
	return -1;
//	return colt_csv::get_next_row();
}

int colt_queuethru::process(int rec_num)
{
	COLT_TRACE("colt_queuethru::process(int rec_num)")
	int rec = 0;

	if(rec_num >= thru_list.size())
		return 1;

	while((rec = thru_list[rec_num]->thru->get_next_row()) >= 0) {
		int index = thru_list[rec_num]->sum + rec;
		colt_csv::process(index);
	}

	return 1;
}
