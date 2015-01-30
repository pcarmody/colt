/*
 * coltisin.h
 *
 *  Created on: Jan 30, 2015
 *      Author: paul
 */

#ifndef COLTISIN_H_
#define COLTISIN_H_

class colt_isin : public colt_each {
	int negative;
public:
	colt_isin(colt_base &in, char *exp, int neg=0);
	virtual ~colt_isin();

	int process(int rec_num);
};

#endif /* COLTISIN_H_ */
