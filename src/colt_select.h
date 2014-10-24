/*
 * colt_select.h
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */

#ifndef COLT_select_H_
#define COLT_select_H_

class colt_select : public colt_operator {
public:
	char 	**headers;
	int		*list;
	int		count;
	char	*colt_select_out[100];

	colt_select(colt_base &in, char **l, int c);
	virtual ~colt_select();

	virtual colt_base *copy(colt_base *op=NULL);

	char **fields(int rec);
	int num_cols();
	char *col_header(int n);
	int preprocess();
};

#endif /* COLT_select_H_ */
