/*
 * coltqueuethru.cpp
 *
 *  Created on: May 1, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltqueuethru.h"

colt_queuethru::colt_queuethru(char *file_name, char col_sep, char eol_sep, char q_char):
	colt_csv(file_name, col_sep, eol_sep, q_char)
{
	// TODO Auto-generated constructor stub

}

colt_queuethru::~colt_queuethru() {
	// TODO Auto-generated destructor stub
}

int colt_queuethru::get_next_index(int curr_index, int size, char *buff)
{
	COLT_TRACE("colt_queuethru::get_next_index(int curr_index, int size, char *buff)")
    while(curr_index < size && buff[curr_index] != '\n'){
    	if(buff[curr_index] == column_sep_char) {
//    		buff[curr_index] = '\0';
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
//    	buff[curr_index] = '\0';
    	return curr_index + 1;
    }

    return 0;
}

char **colt_queuethru::fields(int rec_num)
{
	COLT_TRACE("**colt_queuethru::fields(int rec_num)")
//	if(fields_retval)
//		free(fields_retval);
	_trace.start() << " " << rec_num << ":" << num_lines() << ":" << file_name << "!\n";

	if(rec_num > num_lines())
		rec_num = num_lines();
	_trace.start() << " " << rec_num << "\n";

	int i = 0;
	fields_retval[0] = record(rec_num);
	int number_of_cols = num_cols();
	for(char *x = fields_retval[0]; i<number_of_cols; x++)
		if(!*x) {
			fields_retval[++i] = x+1;
		}

	return fields_retval;
}
