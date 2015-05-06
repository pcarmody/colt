/*
 * coltcsv.h
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */

#ifndef COLTCSV_H_
#define COLTCSV_H_

class colt_csv : public colt_zsv {
public:
	colt_csv(char *file_name, char col_sep='\t', char eol_sep='\n', char q_char='\0');
	virtual ~colt_csv();

	virtual int append_thru(colt_base *thru);

	virtual int	find_sep_chars(int set_sep_chars);
	virtual int load_headers();
	virtual	int get_next_index(int curr_index, int size, char *buff);
	virtual	char **fields(int rec_num);

};

#endif /* COLTCSV_H_ */
