/*
 * coltindex.h
 *
 *  Created on: Apr 20, 2015
 *      Author: paul
 */

#ifndef COLTINDEX_H_
#define COLTINDEX_H_

class colt_index : public colt_operator {
	int *index_list;
	int count;
public:
	colt_index(colt_base &in, int c, int *l);
	virtual ~colt_index();

	virtual int process(int row);
};

#endif /* COLTINDEX_H_ */
