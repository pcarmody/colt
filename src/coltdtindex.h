/*
 * coltdtindex.h
 *
 *  Created on: Nov 6, 2014
 *      Author: paulcarmody
 */

#ifndef COLTDTINDEX_H_
#define COLTDTINDEX_H_

class colt_dt_index : public colt_integer {
	coltthru	*thru;
	char		*thru_file_name;
public:
	colt_dt_index(char *file_name);
	virtual ~colt_dt_index();

//	virtual	void set_buffer(char *x);
//	virtual	int format(char *x);
//	virtual int generate(char *x);
//	virtual	char *consume(char *x);
	virtual int gen_header(char *x);
	coltthru *get_thru();
};

#endif /* COLTDTINDEX_H_ */
