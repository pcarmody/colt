/*
 * coltuniq.h
 *
 *  Created on: Jan 6, 2015
 *      Author: paul
 */

#ifndef COLTUNIQ_H_
#define COLTUNIQ_H_

class colt_uniq : public colt_onchange {
	AssocArray<int>	list;
	int				last;
public:
	colt_uniq(colt_base &in, char *col, int l=0);
	virtual ~colt_uniq();

	int process(int rec_num);
	void postprocess();
};

#endif /* COLTUNIQ_H_ */
