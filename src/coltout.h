/*
 * coltout.h
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */

#ifndef COLTOUT_H_
#define COLTOUT_H_

class colt_out : public colt_operator {
	char	*column_sep_char;
	char	*end_of_line_sep_char;
	char	*quote_char;
public:
	colt_out(colt_base &in, char *col=",", char *eol="\n", char *quote="\"");
	virtual ~colt_out();

	virtual	colt_base *copy(colt_base *operand=NULL);

	void fill_sequential();

	virtual int preprocess();
	virtual int process(int i);
};
#endif
