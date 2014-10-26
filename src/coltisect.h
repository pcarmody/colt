/*
 * coltisect.h
 *
 *  Created on: Aug 4, 2014
 *      Author: paul
 */

#ifndef COLTISECT_H_
#define COLTISECT_H_

class colt_isect : public coltbitmap {
public:
	coltbitmap	*right_operand;

	colt_isect(colt_base &in, colt_base *r);
	colt_isect(colt_base &b, char *file, coltbitmap *r);
	virtual ~colt_isect();

	void set_right(coltbitmap *r) { right_operand = r; };
	void generate(coltbitmap *r);
	void process_all();
};

#endif /* COLTISECT_H_ */
