/*
 * colt_cross.h
 *
 *  Created on: Jun 24, 2014
 *      Author: paulcarmody
 */

#ifndef COLT_CROSS_H_
#define COLT_CROSS_H_

class colt_cross : public colt_each {
	class colt_cross_xrefs {
	public:
		int	left;
		int	right;
		colt_operator *expression;

		colt_cross_xrefs(int l, int r, colt_operator *e):
			left(l),
			right(r),
			expression(e)
		{};
		~colt_cross_xrefs() {};
	};
	IntAssocArray<colt_cross_xrefs *>	xrefs;
public:
	int				current_rec_num;
	char			**field_list;
	colt_datatype	**cell_list;
	char			**cross_headers;
	int				left_num_cols;
	int				right_num_cols;
	colt_operator	*first_expression;

	colt_cross(colt_base&in, char *exp);
//	colt_cross(colt_base &in, colt_base &rite);
//	colt_cross(colt_base &in, colt_base *rite);
	~colt_cross();

	colt_base *copy(colt_base *op=NULL);

	colt_operator *insert_expression(char *expression, int rec_num, int no_destination=1);

	int	num_cols();
	char *col_header(int n);
	char **col_headers();

	virtual int compare(int a, int b, int c);
	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);
	colt_nested_cells *nested_cells(int rec_num);

	int preprocess();
	int process(int rec_num);
};

#endif
