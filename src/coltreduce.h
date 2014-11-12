/*
 * coltreduce.h
 *
 *  Created on: Nov 12, 2014
 *      Author: paulcarmody
 */

#ifndef COLTREDUCE_H_
#define COLTREDUCE_H_

class colt_reduce : public colt_operator {
	int 	level;
	char	*source;
	colt_base	*datasource;
public:
	colt_reduce(colt_base &in, int n);
	colt_reduce(colt_base &in, char *dsource);
	virtual ~colt_reduce();

	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);
	int preprocess();
	int process(int rec_num);
};

#endif /* COLTREDUCE_H_ */
