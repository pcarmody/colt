/*
 * coltadd.h
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#ifndef COLTADD_H_
#define COLTADD_H_

class colt_add : public colt_cexpression {
public:
	char *label;
	int	type;
	char	*colt_add_out[100];
	colt_datatype	*colt_add_cell[100];

//	colt_add(colt_base &in);
	colt_add(colt_base &in, char *col_name, int t, char *tstr);
	virtual ~colt_add();

	char **fields(int rec);
	colt_datatype **cells(int rec);
	int num_cols();
	char *col_header(int n);
	int preprocess();
};

#endif /* COLTADD_H_ */
