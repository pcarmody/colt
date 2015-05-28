/*
 * coltpartthru.cpp
 *
 *  Created on: May 27, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltpartthru.h"

colt_partthru::colt_partthru(char *file_name, char col_sep, char eol_sep, char q_char) :
	colt_queuethru(file_name, col_sep, eol_sep, q_char)
{
	// TODO Auto-generated constructor stub

	i_am = colt_class_partthru;

	meta_file_type = "qthru";
}

colt_partthru::~colt_partthru() {
	// TODO Auto-generated destructor stub
}

colt_nested_cells *colt_partthru::nested_cells(int rec_num)
{

	COLT_TRACE("colt_queuethru::nested_cells(int rec_num)");

	colt_nested_cells *retval = colt_queuethru::nested_cells(rec_num);

	int i;
	int size = thru_list.size();
	int thru_rec_num = rec_num;

	for(i=0; i<size && thru_rec_num > thru_list[i]->thru->num_lines(); i++)
		thru_rec_num -= thru_list[i]->thru->num_lines();

	if(thru_list[i]->thru->num_lines() == thru_rec_num) {
		i++;
		thru_rec_num = 0;
	}

	char **f = colt_csv::fields(i);
	retval->next->key = f[1];
//	retval->num_cols = 1;

//	cout << "qqq " << i << ":" << f[1] << "\n";

	return retval;
//	int i;
//	int size = thru_list.size();
//	int thru_rec_num = rec_num;
//
//	for(i=0; i<size && thru_rec_num > thru_list[i]->thru->num_lines(); i++)
//		thru_rec_num -= thru_list[i]->thru->num_lines();
//
//	if(thru_list[i]->thru->num_lines() == thru_rec_num) {
//		i++;
//		thru_rec_num = 0;
//	}
//
//	if(!nested)
//		nested = new colt_nested_cells(NULL, 0, NULL, source_file_name());
//
//	nested->next = thru_list[i]->thru->nested_cells(thru_rec_num);
////	nested->cells = colt_csv::fields(i);
//
//	return nested;
}
