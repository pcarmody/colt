/*
 * coltmetadata.h
 *
 *  Created on: Apr 22, 2015
 *      Author: paul
 */

#ifndef COLTMETADATA_H_
#define COLTMETADATA_H_

class colt_metadata : public colt_operator {
public:
	colt_metadata(colt_base &in);
	virtual ~colt_metadata();

	int	num_cols();
	char *col_header(int n);
	char **col_headers();
	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);

	int process(int row);
};

#endif /* COLTMETADATA_H_ */
