/*
 * coltreducethru.h
 *
 *  Created on: Jun 9, 2015
 *      Author: paul
 */

#ifndef COLTREDUCETHRU_H_
#define COLTREDUCETHRU_H_

class colt_reduce_thru : public colt_operator {
	colt_base *thru;
public:
	colt_reduce_thru(colt_base &in, char *fname);
	virtual ~colt_reduce_thru();

	int process(int rec_num);
};

#endif /* COLTREDUCETHRU_H_ */
