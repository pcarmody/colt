/*
 * coltshare.h
 *
 *  Created on: Aug 25, 2014
 *      Author: paul
 */

#ifndef COLTSHARE_H_
#define COLTSHARE_H_

class colt_share : public colt_operator {
	vector<colt_operator *>	expressions;
public:
	colt_share(colt_base &in);
	virtual ~colt_share();

	void add_expression(colt_operator *exp);

	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTSHARE_H_ */
