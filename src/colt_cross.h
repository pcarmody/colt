/*
 * colt_cross.h
 *
 *  Created on: Jun 24, 2014
 *      Author: paulcarmody
 */

#ifndef COLT_CROSS_H_
#define COLT_CROSS_H_

class colt_cross : public colt_each {
	IntAssocArray<colt_base *>	elements;
public:
	int				current_rec_num;
	char			**field_list;
	colt_datatype	**cell_list;
	char			**cross_headers;

	colt_cross(colt_base&in, char *exp);
//	colt_cross(colt_base &in, colt_base &rite);
//	colt_cross(colt_base &in, colt_base *rite);
	~colt_cross();

	colt_base *copy(colt_base *op=NULL);

	colt_operator *insert_expression(char *expression, int rec_num, int no_destination=1);

	int	num_cols();
	char *col_header(int n);
	char **col_headers();
	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);

//	int preprocess();
	int process(int rec_num);
};

#endif
