/*
 * coltaggregate.h
 *
 *  Created on: Aug 21, 2014
 *      Author: paul
 */

#ifndef COLTAGGREGATE_H_
#define COLTAGGREGATE_H_

class colt_aggregate : public colt_operator {
	class agg_details_struct {
	public:
		int		function;
		int		column_number;
		char	*column_name;
		agg_details_struct(int func, char *col_name, int col_number=-1) {
			function = func;
			column_name = new char[strlen(col_name)+1];
			strcpy(column_name, col_name);
			column_name[strlen(col_name)] = '\0';
			col_number = col_number;
		};
		virtual ~agg_details_struct() {
			delete column_name;
		}
	};
	vector<agg_details_struct *> agg_details;

	char	**result_fields;
	long	*values;
public:
	enum Function {
		first,
		last,
		max,
		min,
		sum,
		count
	};

	colt_aggregate(colt_base &in);
	colt_aggregate(colt_base &in, int func, char *col_name);
	virtual ~colt_aggregate();

	void add_aggregation(int func, char *col_name, int col_number=-1);

	char **fields(int rec_num);
	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTAGGREGATE_H_ */
