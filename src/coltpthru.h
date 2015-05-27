/*
 * coltpthru.h
 *
 *  Created on: May 26, 2015
 *      Author: paul
 */

#ifndef COLTPTHRU_H_
#define COLTPTHRU_H_

class colt_pthru : public colt_onchange {
	AssocArray<colt_operator *>	list;
	AssocArray<colt_operator *>	end_of_list;
	char 	*filename;
public:
	colt_pthru(colt_base &in, char *exp, char *col, char *f=NULL);
	virtual ~colt_pthru();

	colt_operator *insert_expression(char *expression);
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTPTHRU_H_ */
