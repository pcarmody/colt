/*
 * coltsequentialcsv.h
 *
 *  Created on: Oct 19, 2014
 *      Author: paul
 */

#ifndef COLTSEQUENTIALCSV_H_
#define COLTSEQUENTIALCSV_H_

class colt_sequential_csv : public colt_csv {
	FILE	*fdesc_ptr;
	char	sequential_file_buffer[COLT_MAX_STRING_SIZE];

public:
	colt_sequential_csv(char *file_name, char col_sep, char eol_sep, char q_char);
	colt_sequential_csv(char *file_name, int pl=1);
	virtual ~colt_sequential_csv();

//	virtual colt_base *copy(colt_base *op);

//	void set_quote_char(char q) { quote_char = q; };
//	void set_sep_char(char s) { column_sep_char = s; };
//	char sep_char();
//	int get_next_index(int curr_index, int size, char *buff);
	int open_file(int set_sep_chars=0);
//	int load_headers();
//	int preload_data();
//	char *extract_str(char *in, char sep_char='\t', char eol_char='\n', char quote_char = '\0');
	char *record(int line_num);
//	char *col_header(int n);
	int num_lines();
//	int num_cols();
	int max_size();
//	char *field_val(int rec_num, int col_num);
//	char **fields(int rec_num);
//	bool sort_func(int i, int j);
//	int show_status(int indent_level=0);
//
//	char *source_file_name();

	int get_next_row();
	int preprocess();
};

#endif /* COLTSEQUENTIALCSV_H_ */
