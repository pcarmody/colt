/*
 * coltzsv.h
 *
 *  Created on: May 5, 2015
 *      Author: paul
 */

#ifndef COLTZSV_H_
#define COLTZSV_H_

#define COLT_ZSV_NUM_COLUMN_HEADERS 9

class colt_zsv : public colt_base {
public:
	char 	file_name[100];
	char 	*base_ptr;
	int		fdescriptor;
    int		size;
    std::vector<int> lines;
    char	column_sep_char;
    char	end_of_line_sep_char;
    char	quote_char;
    int 	col_count;
    char	**headers;
    colt_datatype	**cell_objects;
    int		col_iterator;
	char 	**fields_retval;
	int		line_counter;
    int		file_size;
    int 	curr_index;
    int 	next_index;
    int		preload;
    char	**metadata_fields;
    char	*meta_file_type;
    colt_datatype	**metadata_cells;
    colt_nested_cells	*nested;

	colt_zsv(char *file_name, char col_sep, char eol_sep, char q_char);
	colt_zsv(char *file_name, int pl=1);
	virtual ~colt_zsv();

	virtual colt_base *copy(colt_base *op);

	virtual int open_and_load();
	virtual	void set_quote_char(char q) { quote_char = q; };
	virtual	void set_sep_char(char s) { column_sep_char = s; };
	virtual	char sep_char();
	virtual	int get_next_index(int curr_index, int size, char *buff);
	virtual	int open_file(int set_sep_chars=0);
	virtual int find_sep_chars(int set_sep_chars=0);
	virtual	int load_headers();
	virtual	int preload_data();
	virtual	char *extract_str(char *in, char sep_char='\t', char eol_char='\n', char quote_char = '\0');
	virtual	char *record(int line_num);
	virtual	char *col_header(int n);
	virtual char **col_headers();
	virtual	int num_lines();
	virtual	int num_cols();
	virtual	int max_size();
	virtual	char *field_val(int rec_num, int col_num);
	virtual	char **fields(int rec_num);
	virtual void set_coltype(int num, colt_datatype *x);
	virtual void set_datatype(int i, int type);
	virtual	colt_datatype **cells(int rec_num);
	virtual colt_nested_cells *nested_cells(int rec_num);
	virtual	bool sort_func(int i, int j);
	virtual	int show_status(int indent_level=0);
	virtual int compare(int a, int b, int c);
	virtual int contains(int rec_num);
	virtual char *index_file_name();

	virtual int	meta_num_cols();
	virtual char *meta_col_header(int n);
	virtual char **meta_col_headers();
	virtual char **meta_fields(int rec_num);
	virtual colt_datatype **meta_cells(int rec_num);
	virtual	int	get_meta_row(int rec_num);

	virtual	char *source_file_name();

	virtual	int get_next_row();
	virtual int preprocess();
};

#endif /* COLTZSV_H_ */
