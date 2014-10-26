/*
 * coltpartition.h
 *
 *  Created on: Aug 19, 2014
 *      Author: paul
 */

#ifndef COLTPARTITION_H_
#define COLTPARTITION_H_

class colt_partition : public colt_onchange {
	AssocArray<colt_operator *>	list;
	AssocArray<colt_operator *>	end_of_list;
public:
	colt_partition(colt_base &in, char *exp, char *col);
	virtual ~colt_partition();

	colt_operator *insert_expression(char *expression);
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTPARTITION_H_ */
