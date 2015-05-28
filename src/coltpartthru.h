/*
 * coltpartthru.h
 *
 *  Created on: May 27, 2015
 *      Author: paul
 */

#ifndef COLTPARTTHRU_H_
#define COLTPARTTHRU_H_

class colt_partthru : public colt_queuethru {
public:
	colt_partthru(char *file_name, char col_sep='\t', char eol_sep='\n', char q_char='\0');
	virtual ~colt_partthru();

	virtual colt_nested_cells *nested_cells(int rec_num);
};

#endif /* COLTPARTTHRU_H_ */
