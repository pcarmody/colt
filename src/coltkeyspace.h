/*
 * coltkeyspace.h
 *
 *  Created on: Aug 13, 2014
 *      Author: paul
 */

#ifndef COLTKEYSPACE_H_
#define COLTKEYSPACE_H_

class colt_keyspace : public colt_sort {
//	int column_number;
	char **column_value;
public:
	colt_keyspace(colt_base &in, int col, char *destination_file_name=NULL);
	colt_keyspace(char *source=NULL);
	virtual ~colt_keyspace();

	int serialize_id_num();

	int num_cols();
	char *col_header(int n);
	char **col_headers();
	char **fields(int rec_num);
	int preprocess();
	int process(int rec_num);
	void perform_sort();
};

#endif /* COLTKEYSPACE_H_ */
