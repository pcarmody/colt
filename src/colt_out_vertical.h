/*
 * colout_vertical.h
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */

#ifndef COLTout_vertical_H_
#define COLTout_vertical_H_

class colt_out_vertical : public colt_out {
	char 	col_seperator;
	int		column_length;
public:
	colt_out_vertical(colt_base &in, char sp='|');
	virtual ~colt_out_vertical();

	virtual colt_base *copy(colt_base *op=NULL);

	void fill_sequential();

	virtual int preprocess();
	virtual int process(int rec);
};
#endif
