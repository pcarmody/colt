/*
 * colt_skip_limit.h
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */

#ifndef COLT_skip_limit_H_
#define COLT_skip_limit_H_

class colt_skip_limit : public colt_operator {
public:
	int			skip_by;
	int			limit_to;
	int			rec_counter;

	colt_skip_limit(colt_base &in, int limit=1, int skip=0);
	virtual ~colt_skip_limit();

	virtual colt_base *copy(colt_base *op=NULL);

	void fill_sequential(int start=0);
	int num_lines();

	int preprocess();
//	char **fields(int rec);
	int process(int rec);
};

#endif /* COLT_skip_limit_H_ */
