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
	colt_class_csv,
	colt_class_sequential_csv,

	// output formats
	colt_class_out,
	colt_class_html,
	colt_class_out_vertical,
	colt_class_to_json,

	colt_class_counter,

	// row operations
	colt_class_skip_limit,
	colt_class_select,
	colt_class_if,
	colt_class_aggregate,
	colt_class_aggregate_row,

	// compound expressions
	colt_class_each,
	colt_class_partition,
	colt_class_onchange,
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
	colt_exception() {};
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
	virtual	colt_base *copy_expression(colt_base *op=NULL);
	virtual colt_base *copy(colt_base *op=NULL);

	virtual void set_destination(colt_base *obj, int bothways=1);
	virtual colt_base *get_destination();
	virtual	void set_operand(colt_base *);
	virtual	char *record(int line_num);
	virtual char *col_header(int n);
	virtual int num_lines();
	virtual int num_cols();
	virtual int max_size();
	virtual	char *field_val(int rec_num, int col_num);
	virtual	char **fields(int rec_num);
	virtual char *extract_str(char *in, char sep_char='\t', char eol_char='\n', char quote_char = '\0');
	virtual char find_sep_char(char *row, char end_char='\n');
	virtual char sep_char();
	virtual char *source_file_name();

	virtual	void process_all();
	virtual	int get_next_row();
	virtual int preprocess();
	virtual void postprocess();
	virtual int process(int rec);
};

#endif /* COLTBASE_H_ */
