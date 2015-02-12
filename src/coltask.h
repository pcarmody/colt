/*
 * coltask.h
 *
 *  Created on: Feb 9, 2015
 *      Author: paul
 */

#ifndef COLTASK_H_
#define COLTASK_H_

class colt_ask : public colt_base {
	int 	sockfd;
	char	*colt_expression;
	char	*hostname;
	char	*port;
    int 	column_count;
    char	**headers;
    char	**ask_fields;
public:
	colt_ask(char *cxp, char *host, char *p);
	virtual ~colt_ask();

	int num_cols();
	char **col_headers();
	int get_next_row();
	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);

	int preprocess();
//	int process(int rec_num);
	void postprocess();
};

#endif /* COLTASK_H_ */
