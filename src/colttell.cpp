/*
 * colttell.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "colttell.h"
#include <unistd.h>

colt_tell::colt_tell(int sock):
	colt_operator(),
	socket_desc(sock),
	recursive(0),
	expression(NULL)
{
	// TODO Auto-generated constructor stub
	char buff[COLT_MAX_STRING_SIZE];
	int len = read(socket_desc, buff, COLT_MAX_STRING_SIZE);

	colt_parser parse(buff);
	out_object = parse.parse();
	expression = out_object->get_destination();

	int ok = (expression) ? -1: 0;
	len = write(socket_desc, &ok, sizeof(ok));

	out_object->set_destination(this);
	operand = out_object;

	cout << "Executing '" << buff << "'\n";
}

colt_tell::~colt_tell() {
	// TODO Auto-generated destructor stub
}

int colt_tell::get_next_row()
{
	cout << "qqq colt_tell::get_next_row()\n";

	if(!expression)
		return -1;

	return expression->get_next_row();
}

void colt_tell::fatal_error(char *m)
{
	perror(m);
	exit(-1);
}

char **colt_tell::fields(int rec_num)
{
	cout << "qqq fields\n";
	return colt_operator::fields(rec_num);
}

int colt_tell::preprocess()
{
	cout << "qqq colt_tell::preprocess()\n";
	int cols = expression->num_cols();
	int n;
	n = write(socket_desc, &cols, sizeof(cols));
	if(n < 0)
		fatal_error("Tell write num_cols");
	cout << "qqq colt_tell::preprocess()  A " << (long) expression << "\n";

	int retval = colt_operator::preprocess();

	char **headers = expression->col_headers();
	cout << "qqq colt_tell::preprocess()  B\n";

	for(int i=0; i<cols; i++) {
		cout << "qqq colt_tell::preprocess() C\n";
		int ok;
		n = write(socket_desc, headers[i], strlen(headers[i])+1);
		if(n < 0)
			fatal_error("Tell send headers");
		read(socket_desc, &ok, sizeof(ok));
	}

	cout << "qqq exiting preprocess()\n";

	return retval;//out_object->preprocess();
}

int colt_tell::process(int rec_num)
{
	cout << "qqq colt_tell::process()\n";
	if(recursive) {
		recursive = 0;
		char **tell_fields = expression->fields(rec_num);
		int cols = expression->num_cols();
		cout << "qqq " << rec_num << ":" << cols << "\n";
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

	cout << "qqq " << rec_num << "<<<<<<<\n";
	recursive = 1;

	return out_object->process(rec_num);
}

void colt_tell::postprocess()
{
	char end_of_data[10];
	strcpy(end_of_data, "__EOD__");
	int n = write(socket_desc, end_of_data, strlen(end_of_data)+1);
	if(n < 0)
		fatal_error("Tell send headers");
}
