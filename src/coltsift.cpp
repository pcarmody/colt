/*
 * coltsift.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltsift.h"

colt_sift::colt_sift(colt_base &in, char *fname, char *iname, char *cname):
	colt_operator(in),
	source(NULL),
	source_name(NULL),
	index_name(NULL),
	col_name(NULL),
	col_number(-1)
{
	// TODO Auto-generated constructor stub
	source_name = new char[strlen(fname)+1];
	strcpy(source_name, fname);
	index_name = new char[strlen(iname)+1];
	strcpy(index_name, iname);
	col_name = new char[strlen(cname)+1];
	strcpy(col_name, cname);
}

colt_sift::~colt_sift()
{
	// TODO Auto-generated destructor stub
}

int colt_sift::preprocess()
{
	COLT_TRACE("colt_sift::preprocess()")
	for(int j=0; j<colt_operator::num_cols(); j++) {
		char *head = operand->col_header( j );
		if(strcmp(col_name, head ) == 0 ) {
			col_number = j;
			break;
		}
	}

	if(col_number == -1) {
		cout << "sift couldn't find column '" << col_name << "'.\n";
		throw "Could not find column key for onchange.\n";
	}
	_trace.start() << " source name is " << source_name << "\n";
	source = colt_load_thru(source_name);

	if(!source)
		source = new colt_csv(source_name);

	_trace.start() << " source is a sort: " << source->is_a(colt_class_sort) << "\n";
	if(!source->is_a(colt_class_sort)) {
		source = new colt_cthru(*source, 0);
		source->process_all();
	}

	return colt_operator::preprocess();
}

int colt_sift::process(int rec_num)
{
	COLT_TRACE("colt_sift::process(int rec_num)")

	colt_datatype **row = cells(rec_num);
	char search_value[COLT_MAX_STRING_SIZE];
	cells(rec_num)[col_number]->format(search_value);

	colt_sort *tmp = (colt_sort *) source;

	if(tmp->search(search_value, search_value))
		return colt_operator::process(rec_num);

	return 0;
}

void colt_sift::postprocess()
{
	COLT_TRACE("colt_sift::process(int rec_num)")

	colt_operator::postprocess();
}
