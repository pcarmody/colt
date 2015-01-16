/*
 * coltbash.h
 *
 *  Created on: Jan 14, 2015
 *      Author: paul
 */

#ifndef COLTBASH_H_
#define COLTBASH_H_

class colt_bash : public colt_add {
	char	bash_command[COLT_MAX_STRING_SIZE];
public:
	colt_bash(colt_base &in, char *col_name, int t, char *tstr);
	virtual ~colt_bash();

	void get_value(int rec_num=0);
};

#endif /* COLTBASH_H_ */

