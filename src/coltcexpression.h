/*
 * coltcexpression.h
 *
 *  Created on: Nov 7, 2014
 *      Author: paulcarmody
 */

#ifndef COLTCEXPRESSION_H_
#define COLTCEXPRESSION_H_

typedef int(*COLT_C_FUNC)(void **row);

class colt_cexpression : public colt_operator {
	static int expression_count;
public:
	COLT_C_FUNC		function_ptr;
	char		*code_string;
	char		*return_type;
	char		*lookup_key;

	colt_cexpression(colt_base &in, char *exp, char *str=NULL);
	colt_cexpression(colt_base &in, COLT_C_FUNC func);
	virtual ~colt_cexpression();

	void compile_and_link();

	int preprocess();
};

#endif /* COLTCEXPRESSION_H_ */
