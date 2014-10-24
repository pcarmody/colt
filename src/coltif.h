/*
 * coltif.h
 *
 *  Created on: May 8, 2014
 *      Author: paulcarmody
 */

#ifndef COLTIF_H_
#define COLTIF_H_

typedef int(*COLT_IF_FUNC)(char **row);

class colt_if : public colt_operator {
	COLT_IF_FUNC	condition;
	char		*code_string;
public:
	colt_if(colt_base &b, COLT_IF_FUNC cond);
	colt_if(colt_base &b, char *c);
	virtual ~colt_if();

//	char **fields(int rec);
	int process(int rec);
};

#endif /* COLTIF_H_ */
