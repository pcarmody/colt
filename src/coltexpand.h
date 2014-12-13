/*
 * coltexpand.h
 *
 *  Created on: Nov 6, 2014
 *      Author: paulcarmody
 */

#ifndef COLTEXPAND_H_
#define COLTEXPAND_H_

class colt_expand : public colt_cross {
	int 			col_number;
	char			*col_name;
	char			*expression;
	colt_base		*dt_thru;
public:
	colt_expand(colt_base &in, char *cname, char *exp);
	virtual ~colt_expand();

	int preprocess();
	int process(int rec_num);
};

#endif /* COLTEXPAND_H_ */
