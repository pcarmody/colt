/*
 * coltaggregagerow.h
 *
 *  Created on: Aug 22, 2014
 *      Author: paul
 */

#ifndef COLTAGGREGAGEROW_H_
#define COLTAGGREGAGEROW_H_

class colt_aggregate_row : public colt_operator {
	int		count;
	int		*rows;
	int		function;
	char	*column_name;
	int		column_number;
public:
	colt_aggregate_row(colt_base &in, int func, int cnt=1, char *col_name=NULL);
	virtual ~colt_aggregate_row();

	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTAGGREGAGEROW_H_ */
