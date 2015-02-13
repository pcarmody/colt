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
	char	**selected_headers;
	int		*list;
	int		count;
	char	*colt_select_out[100];
	colt_datatype	*colt_select_cells[100];
	int		no_match;

	colt_select(colt_base &in, char **l, int c, int nom=0);
	virtual ~colt_select();

	char **fields(int rec);
	colt_datatype **cells(int rec);
	int num_cols();
	char *col_header(int n);
	char **col_headers();

	int preprocess();
	int process(int rec_num);
};

#endif /* COLT_select_H_ */
