/*
 * coltoperator.h
 *
 *  Created on: May 7, 2014
 *      Author: paulcarmody
 */
#include "coltbase.h"

#ifndef COLTOPERATOR_H_
#define COLTOPERATOR_H_

class colt_operator : public colt_base {
public:
	colt_base	*operand;

	colt_operator(colt_base &in);
	colt_operator();
	virtual ~colt_operator();

	int is_a(Colt_Class c);

	virtual	void set_operand(colt_base *);
	virtual	void set_destination(colt_base *obj, int bothways=1);

	char *record(int line_num);
	char *col_header(int n);
	char *field_val(int rec_num, int col_num);
	char *extract_str(char *in, char sep_char='\t', char eol_char='\n');
	virtual char sep_char();
	virtual int num_lines();
	virtual int num_cols();
	virtual int max_size();
	virtual char **fields(int rec_num);
	virtual void set_coltype(int num, colt_datatype *x);
	virtual	colt_datatype **cells(int rec_num);
	virtual char *source_file_name();

	virtual	int get_next_row();
//	virtual int preprocess();
//	virtual void postprocess();
//	virtual int process(int rec);
};

#endif /* COLTOPERATOR_H_ */
