/*
 * coltcounter.h
 *
 *  Created on: Aug 6, 2014
 *      Author: paul
 */

#ifndef COLTCOUNTER_H_
#define COLTCOUNTER_H_

class colt_counter : public colt_out {
	int counter;
public:
	colt_counter(colt_base &b);
	virtual ~colt_counter();

	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTCOUNTER_H_ */
