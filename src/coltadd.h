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
	char 			*label;
	int				type;
	char			value[COLT_MAX_STRING_SIZE];
	char			*colt_add_out[COLT_MAX_NUM_COLS];
	colt_datatype	*colt_add_cell[COLT_MAX_NUM_COLS];
	char 			*headers[COLT_MAX_NUM_COLS];

//	colt_add(colt_base &in);
	colt_add(colt_base &in, char *col_name, int t, char *tstr);
	virtual ~colt_add();

	virtual void get_value(int rec_num=0);

	char **fields(int rec);
	colt_datatype **cells(int rec);
	int num_cols();
	char *col_header(int n);
	virtual char **col_headers();
	int preprocess();
};

#endif /* COLTADD_H_ */
