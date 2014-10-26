/*
 * coltunion.h
 *
 *  Created on: Aug 7, 2014
 *      Author: paul
 */

#ifndef COLTUNION_H_
#define COLTUNION_H_

class colt_union : public colt_isect {
public:
	colt_union(colt_base &in, colt_base *r);
	colt_union(colt_base &b, char *file, coltbitmap *r);
	virtual ~colt_union();

	void generate(coltbitmap *r);
};

#endif /* COLTUNION_H_ */
