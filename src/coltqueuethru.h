/*
 * coltqueuethru.h
 *
 *  Created on: May 1, 2015
 *      Author: paul
 */

#ifndef COLTQUEUETHRU_H_
#define COLTQUEUETHRU_H_

class colt_queuethru : public colt_csv {
public:
	colt_queuethru(char *file_name, char col_sep='\t', char eol_sep='\n', char q_char='\0');
	virtual ~colt_queuethru();

	virtual	int get_next_index(int curr_index, int size, char *buff);
	virtual	char **fields(int rec_num);
};

#endif /* COLTQUEUETHRU_H_ */
