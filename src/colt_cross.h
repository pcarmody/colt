/*
 * colt_cross.h
 *
 *  Created on: Jun 24, 2014
 *      Author: paulcarmody
 */

#ifndef COLT_CROSS_H_
#define COLT_CROSS_H_

class colt_cross : public colt_operator {
public:
	colt_base		*right;
	int				current_rec_num;
	char			**field_list;
	colt_datatype	**cell_list;

	colt_cross(colt_base &in, colt_base &rite);
	colt_cross(colt_base &in, colt_base *rite);
	~colt_cross();

	colt_base *copy(colt_base *op=NULL);

	int	num_cols();
	char *col_header(int n);
	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);

//	int preprocess();
	int process(int rec_num);
};

#endif
