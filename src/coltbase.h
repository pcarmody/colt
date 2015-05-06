/*
 * coltbase.h
 *
 *  Created on: May 7, 2014
 *      Author: paulcarmody
 */

#ifndef COLTBASE_H_
#define COLTBASE_H_

#define COLT_MAX_STRING_SIZE 1024*4
#define COLT_MAX_NUM_COLS	 1000

enum Colt_Class {

	colt_class_base,
	colt_class_operator,

	// input sources
	colt_class_zsv,
	colt_class_csv,
	colt_class_sequential_csv,

	// output formats
	colt_class_out,
	colt_class_html,
	colt_class_out_vertical,
	colt_class_to_json,
	colt_class_qthru,

	colt_class_counter,

	// row operations
	colt_class_skip_limit,
	colt_class_select,
	colt_class_c_expression,
	colt_class_add,
	colt_class_add_source,
	colt_class_if,
	colt_class_aggregate,
	colt_class_aggregate_row,

	// compound expressions
	colt_class_each,
	colt_class_isin,
	colt_class_partition,
	colt_class_onchange,
	colt_class_uniq,
	colt_class_share,
	colt_class_thread,
	colt_class_sync,

	// thrus

	colt_class_thru,
	colt_class_sort,
	colt_class_keyspace,
	colt_class_cthru,
	colt_class_range,
	colt_class_range_list,
	colt_class_bitmap,
	colt_class_queuethru,

	// binary operations
	colt_class_cross,
	colt_class_isect,
	colt_class_union,
	colt_class_complement,

	// misc
	colt_class_parser
};

class colt_exception {
public:
	int level;
	string message;

	enum {
		STANDARD,
		FATAL
	};
	colt_exception() { level = 0; };
	colt_exception(int l, string &msg) { level = l; message = msg; };
	~colt_exception() {};
};

class colt_base {
	static char extraction_string[10000];
public:
	colt_base	*out_object;
	Colt_Class	i_am;

	colt_base();
	virtual ~colt_base();

	virtual int is_a(Colt_Class c);
	virtual int is_a_thru();
	virtual	colt_base *copy_expression(colt_base *op=NULL);
	virtual colt_base *copy(colt_base *op=NULL);

	virtual int same_index_source(colt_base *xx) { return strcmp(source_file_name(), xx->index_file_name()) == 0; };

	virtual void set_destination(colt_base *obj, int bothways=1);
	virtual colt_base *get_destination();
	virtual	void set_operand(colt_base *);
	virtual	char *record(int line_num);
	virtual char *col_header(int n);
	virtual char **col_headers();
	virtual int num_lines();
	virtual int num_cols();
	virtual int max_size();
	virtual	char *field_val(int rec_num, int col_num);
	virtual	char **fields(int rec_num);
	virtual void set_coltype(int num, colt_datatype *x);
	virtual	colt_datatype **cells(int rec_num);
	virtual char *extract_str(char *in, char sep_char='\t', char eol_char='\n', char quote_char = '\0');
	virtual char find_sep_char(char *row, char end_char='\n');
	virtual char sep_char();
	virtual char *source_file_name();
	virtual char *index_file_name();
	virtual	colt_base *get_datasource(int count=0);
	virtual int	reduce_to(int level, int rec_num);
	virtual int compare(int a, int b, int c);
	virtual int contains(int rec_num);

	virtual int	meta_num_cols();
	virtual char *meta_col_header(int n);
	virtual char **meta_col_headers();
	virtual char **meta_fields(int rec_num);
	virtual colt_datatype **meta_cells(int rec_num);
	virtual	int	get_meta_row(int rec_num);

	virtual	void process_all();
	virtual	int get_next_row();
	virtual int preprocess();
	virtual void postprocess();
	virtual int process(int rec);
};

#endif /* COLTBASE_H_ */
