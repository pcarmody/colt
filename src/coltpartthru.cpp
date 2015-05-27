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

