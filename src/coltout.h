/*
 * coltout.h
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */

#ifndef COLTOUT_H_
#define COLTOUT_H_

class colt_out : public colt_operator {
public:
	char	*column_sep_char;
	char	*end_of_line_sep_char;
	char	*quote_char;
	int		gen_headers;


	colt_out(colt_base &in, char *col=",", char *eol="\n", char *quote=NULL);
	virtual ~colt_out();

	virtual int headers_out();
	virtual int preprocess();
	virtual int process(int i);
};
#endif
