/*
 * coltqueuethru.h
 *
 *  Created on: May 1, 2015
 *      Author: paul
 */

#ifndef COLTQUEUETHRU_H_
#define COLTQUEUETHRU_H_

class colt_queuethru : public colt_csv {
	vector<colt_base *>		thru_list;
	int						queue_index;

public:
	colt_queuethru(char *file_name, char col_sep='\t', char eol_sep='\n', char q_char='\0');
	virtual ~colt_queuethru();

	virtual int append_thru(colt_base *thru);

	virtual int open_and_load();
	virtual int	find_sep_chars(int set_sep_chars);
	virtual int load_headers();
	virtual	int get_next_index(int curr_index, int size, char *buff);
//	virtual int get_next_row();

//	virtual	char *record(int line_num);
//	virtual	char *col_header(int n);
//	virtual char **col_headers();
//	virtual	int num_lines();
//	virtual	int num_cols();
//	virtual	int max_size();
	virtual	char **fields(int rec_num);

//	virtual int	meta_num_cols();
//	virtual char *meta_col_header(int n);
//	virtual char **meta_col_headers();
//	virtual char **meta_fields(int rec_num);
//	virtual colt_datatype **meta_cells(int rec_num);
//	virtual	int	get_meta_row(int rec_num);

};

#endif /* COLTQUEUETHRU_H_ */
