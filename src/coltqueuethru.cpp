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
	thru_list()
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_queuethru;
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

//	for(i = 0; i<size-1 && thru_list[i]->sum < rec_num; i++);
//
//	int thru_rec_num = (i == 0)? rec_num : rec_num - thru_list[i-1]->sum-1;

	return thru_list[i]->thru->fields(thru_rec_num);
}

int colt_queuethru::process(int rec_num)
{
	COLT_TRACE("colt_queuethru::process(int rec_num)")
	int rec = 0;

	if(rec_num >= thru_list.size())
		return 1;

	while((rec = thru_list[rec_num]->thru->get_next_row()) >= 0) {
		int index = thru_list[rec_num]->sum + rec;
//		cout << "qqq " << rec_num << ":" << index << ":" << thru_list[rec_num]->sum << ":" << rec << "\n";
		colt_csv::process(index);
	}

	return 1;
}
