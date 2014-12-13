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
public:
	colt_link(colt_base &in, char *colname, char *exp);
	virtual ~colt_link();

	int process(int rec_num);
};

#endif /* COLTLINK_H_ */
