/*
 * coltonchange.h
 *
 *  Created on: Sep 7, 2014
 *      Author: paul
 */

#ifndef COLTONCHANGE_H_
#define COLTONCHANGE_H_

class colt_onchange: public colt_each {
public:
	int		column_number;
	char	*column_name;
	char	*current_value;

	colt_onchange(colt_base &in, char *exp, char *col);
	virtual ~colt_onchange();

	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTONCHANGE_H_ */
