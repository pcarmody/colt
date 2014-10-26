/*
 * colteach.h
 *
 *  Created on: Aug 15, 2014
 *      Author: paul
 */

#ifndef COLTEACH_H_
#define COLTEACH_H_

class colt_each : public colt_operator {
protected:
	char			*expression_string;
	colt_operator	*expression_object;
	colt_base		*destination;
public:
	colt_each(colt_base &in, char *exp);
	virtual ~colt_each();

	virtual colt_operator *insert_expression(char *expression);
	int process(int rec);
};

#endif /* COLTEACH_H_ */
