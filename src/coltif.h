/*
 * coltif.h
 *
 *  Created on: May 8, 2014
 *      Author: paulcarmody
 */

#ifndef COLTIF_H_
#define COLTIF_H_

//typedef int(*COLT_IF_FUNC)(char **row);

class colt_if : public colt_cexpression {
public:
//	colt_if(colt_base &b, COLT_C_FUNC cond);
	colt_if(colt_base &b, char *c, char *str=NULL);
	virtual ~colt_if();

//	char **fields(int rec);
	void get_value(int rec_num=0);
	int process(int rec);
};

#endif /* COLTIF_H_ */
