/*
 * coltcomplement.h
 *
 *  Created on: Aug 9, 2014
 *      Author: paul
 */

#ifndef COLTCOMPLEMENT_H_
#define COLTCOMPLEMENT_H_

class colt_complement : public colt_isect {
public:
	colt_complement(colt_base &n, coltbitmap *r);
	virtual ~colt_complement();

	void generate(coltbitmap *l);
};

#endif /* COLTCOMPLEMENT_H_ */
