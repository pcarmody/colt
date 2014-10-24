/*
 * coltadd.h
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#ifndef COLTADD_H_
#define COLTADD_H_

class colt_add : public colt_operator {
public:
	char *label;
	int	type;
	char replacement_string;
	char	*colt_add_out[100];
	colt_base *template_str;

	colt_add(colt_base &in);
	colt_add(colt_base &in, char *col_name, int t, char *tstr);
	virtual ~colt_add();

	char **fields(int rec);
	int num_cols();
	char *col_header(int n);
	int preprocess();
};

#endif /* COLTADD_H_ */
