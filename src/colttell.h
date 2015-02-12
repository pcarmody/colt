/*
 * colttell.h
 *
 *  Created on: Feb 9, 2015
 *      Author: paul
 */

#ifndef COLTTELL_H_
#define COLTTELL_H_

class colt_tell : public colt_operator {
	int socket_desc;
//	char *colt_expression;
	colt_base *expression;
	int recursive;
public:
	colt_tell(int sock);
	virtual ~colt_tell();

	void fatal_error(char *m);

	char **fields(int rec_num);
	int get_next_row();
	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTTELL_H_ */
