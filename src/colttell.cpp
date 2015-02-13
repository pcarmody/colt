/*
 * colttell.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "colttell.h"
#include <unistd.h>

colt_tell::colt_tell(colt_base &in, int sock):
	colt_operator(in),
	socket_desc(sock)
{
}

colt_tell::~colt_tell() {
	// TODO Auto-generated destructor stub
}

int colt_tell::get_next_row()
{
	return colt_operator::get_next_row();
}

void colt_tell::fatal_error(char *m)
{
	perror(m);
	exit(-1);
}

char **colt_tell::fields(int rec_num)
{
	COLT_TRACE("*colt_tell::fields(int rec_num)")
	return colt_operator::fields(rec_num);
}

int colt_tell::preprocess()
{
	COLT_TRACE("*colt_tell::preprocess()")
	int cols = num_cols();
	int n;
	n = write(socket_desc, &cols, sizeof(cols));
	if(n < 0)
		fatal_error("Tell write num_cols");

	char **headers = col_headers();

	for(int i=0; i<cols; i++) {
		int ok;
		n = write(socket_desc, headers[i], strlen(headers[i])+1);
		if(n < 0)
			fatal_error("Tell send headers");
		read(socket_desc, &ok, sizeof(ok));
	}

	return colt_operator::preprocess();
}

int colt_tell::process(int rec_num)
{
	COLT_TRACE("*colt_tell::process(int rec_num)")

	char **tell_fields = fields(rec_num);
	int cols = num_cols();
	for(int i=0; i<cols; i++) {
		int ok;
		int n = write(socket_desc, tell_fields[i], strlen(tell_fields[i])+1);
		if(n < 0)
			fatal_error("Tell send headers");
		n = read(socket_desc, &ok, sizeof(ok));
		if(n < 0)
			fatal_error("Tell can't verify sent row.");
	}

	return 1;//out_object->process(rec_num);
}

void colt_tell::postprocess()
{
	COLT_TRACE("*colt_tell::postprocess()")
	char end_of_data[10];
	strcpy(end_of_data, "__EOD__");
	int n = write(socket_desc, end_of_data, strlen(end_of_data)+1);
	if(n < 0)
		fatal_error("Tell send headers");
}
