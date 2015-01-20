/*
 * coltaddsource.h
 *
 *  Created on: Jan 14, 2015
 *      Author: paul
 */

#ifndef COLTADDSOURCE_H_
#define COLTADDSOURCE_H_

class colt_add_source : public colt_add {
	char code_string[COLT_MAX_STRING_SIZE];
	IntAssocArray<coltthru *>	elements;

public:
	colt_add_source(colt_base &in, char *col_name, int t, char *tstr);
	virtual ~colt_add_source();

	void get_value(int rec_num=0);
};

#endif /* COLTADDSOURCE_H_ */
