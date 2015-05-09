/*
 * coltqthru.cpp
 *
 *  Created on: May 4, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltqthru.h"

colt_qthru::colt_qthru(colt_base &in, char *fname):
	colt_operator(in),
	qthru(NULL),
	last_thru(NULL),
	new_thru(0)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_qthru;
	file_name = new char[strlen(fname)+1];
	strcpy(file_name, fname);
}

colt_qthru::~colt_qthru() {
	// TODO Auto-generated destructor stub
	delete qthru;
	delete file_name;

	if(last_thru)
		delete last_thru;
}

int colt_qthru::preprocess()
{
	FILE *exists = fopen(file_name, "r");
	if(exists)
		fclose(exists);
	else {
		FILE *new_queuethru = fopen(file_name, "w");
		fprintf(new_queuethru, "%s\n", operand->source_file_name());
		fclose(new_queuethru);
	}

	qthru = new colt_queuethru(file_name);

	if(operand->is_a_thru())
		return colt_operator::preprocess();

	if(qthru->num_lines() == 0) {
		new_thru = 1;
		last_thru = new coltthru();
	} else {
		colt_datatype **cells = qthru->cells( qthru->num_lines() );
		last_thru = (coltthru *) cells[0]->get_value();
	}

	return colt_operator::preprocess();
}

int colt_qthru::process(int rec_num)
{
	if(last_thru)
		return last_thru->process(rec_num);

	return 1;
}

void colt_qthru::postprocess()
{
	if(new_thru)
		qthru->append_thru(last_thru);
	else {
		qthru->append_thru(operand);
	}

	return colt_operator::postprocess();
}

