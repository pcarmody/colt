/*
 * coltlink.h
 *
 *  Created on: Nov 11, 2014
 *      Author: paulcarmody
 */

#ifndef COLTLINK_H_
#define COLTLINK_H_

class colt_link : public colt_each {
	char *column_name;
	int	datatype;
public:
	colt_link(colt_base &in, char *colname, int type, char *exp);
	virtual ~colt_link();
};

#endif /* COLTLINK_H_ */
