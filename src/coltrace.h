/*
 * coltrace.h
 *
 *  Created on: Jan 26, 2015
 *      Author: paul
 */

#ifndef COLTRACE_H_
#define COLTRACE_H_

class colt_race : public colt_operator {
	class colt_race_xref {
	public:
		int	left;
		int	right;
		colt_race_xref(int l, int r):
			left(l),
			right(r)
		{};
		~colt_race_xref() {};
	};
	vector<colt_race_xref *>	xrefs;

	char 	*command_string;
	colt_base	*right_expression;
	char 	*label;
	char 	*left_sort_col_name;
	int		left_sort_col;
	char	*right_sort_col_name;
	int		right_sort_col;
	int		onequal_read_left;
	int		onequal_read_right;
	int		left_rec_num;
	int		right_rec_num;
	int		left_num_cols;
	int		right_num_cols;
	colt_datatype **left_cells;
	colt_datatype **right_cells;
	char	**race_headers;
	colt_datatype	**race_cells;
	char	**race_fields;


public:
	colt_race(colt_base &in, char *col_name, char *left_name, char *right_name, char *flags, char *tstr);
	virtual ~colt_race();

	char **fields(int rec_num);
	colt_datatype **cells(int rec);
	int num_cols();
	char *col_header(int n);
	virtual char **col_headers();

	int index_and_process();

	int preprocess();
	int process(int rec_num);
//	void postprocess();
};

#endif /* COLTRACE_H_ */
