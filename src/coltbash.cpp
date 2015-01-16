/*
 * colt_bash.cpp
 *
 *  Created on: Jan 14, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltbash.h"

colt_bash::colt_bash(colt_base &in, char *col_name, int t, char *tstr):
	colt_add(in, col_name, t, tstr)
{
	// TODO Auto-generated constructor stub
	strcpy(bash_command, tstr);
}

colt_bash::~colt_bash() {
	// TODO Auto-generated destructor stub
}

void colt_bash::get_value(int rec_num)
{
	COLT_TRACE("colt_bash::get_value(int rec_num)")
	char pipe_string[COLT_MAX_STRING_SIZE];

	pipe_string[0] = '\0';

	char **heads = colt_operator::col_headers();
	char **strs = colt_operator::fields(rec_num);
	int cols = colt_operator::num_cols();

	for(int i=0; i<cols; i++) {
		char tmp[COLT_MAX_STRING_SIZE];
		sprintf(tmp, "%s='%s' ; ", heads[i], strs[i]);
		strcat(pipe_string, tmp);
	}

	strcat(pipe_string, bash_command);
//	strcat(pipe_string, "\n");

	FILE *pipe = popen(pipe_string, "r");

	char tmp[COLT_MAX_STRING_SIZE];
	value[0] = '\0';
	while(fgets(tmp, COLT_MAX_STRING_SIZE, pipe) != NULL)
		if(strlen(tmp)+strlen(value) > COLT_MAX_STRING_SIZE)
			break;
		else
			strcat(value, tmp);

	if(pclose(pipe)) {
		perror("Bash command: Unable to execute.\n");
		perror(pipe_string);
		exit(1);
	}
}
