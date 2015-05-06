/*
 * coltsequentialcsv.h
 *
 *  Created on: Oct 19, 2014
 *      Author: paul
 */

#ifndef COLTSEQUENTIALCSV_H_
#define COLTSEQUENTIALCSV_H_

class colt_sequential_csv : public colt_zsv {
	FILE	*fdesc_ptr;
	char	sequential_file_buffer[COLT_MAX_STRING_SIZE];

public:
	colt_sequential_csv(char *file_name, char col_sep, char eol_sep, char q_char);
	colt_sequential_csv(char *file_name, int pl=1);
	virtual ~colt_sequential_csv();

	int open_file(int set_sep_chars=0);
	int  load_headers();
	char *record(int line_num);
	int num_lines();
	int max_size();

	int get_next_row();
	int preprocess();
};

#endif /* COLTSEQUENTIALCSV_H_ */
