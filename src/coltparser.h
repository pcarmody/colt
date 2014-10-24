/*
 * coltparser.h
 *
 *  Created on: Jul 23, 2014
 *      Author: paul
 */

#ifndef COLTPARSER_H_
#define COLTPARSER_H_

struct StrFragment {
	char substring[100];
	int index;
};

class colt_parser {
public:
	colt_base	*return_value;
	char		*input_buffer;
	struct StrFragment *substrings;
	int 		numfrags = 0;
	char		compiled_string[COLT_MAX_STRING_SIZE];

	colt_parser(char *);
	virtual ~colt_parser();

	int find_insertions(char **cols);
	char *replace_strings(char **vals);
//	char *replace_strings(colt_datatype **types);

	int is_a(Colt_Class c);
	void fatal_error(char const *err);
	int consume_whitespace();
	int is_token(const char *);
	int consume_token(char *);
	char *consume_keyword(char *);
	int consume_integer();
	int consume_code_segment(char *);

	colt_counter		*count();
	colt_out_vertical	*vertical();
	colt_html 			*html();
	colt_to_json		*json();
	colt_out			*csv();
	colt_base			*output_expression();

	colt_select 		*select();
	colt_skip_limit		*skip_limit();
	colt_if				*ifx();
	colt_add			*add();
	colt_aggregate		*aggregate();
	colt_aggregate_row	*aggregate_row();
	colt_sort			*sort();
	colt_keyspace		*keyspace();
	colt_cthru			*cthru();
	colt_range			*search();
	coltbitmap			*set();
	colt_each			*each();
	colt_partition		*partition();
	colt_onchange		*onchange();
	colt_share			*share();
	colt_thread 		*threadx();
	colt_sync			*sync();
	colt_base			*unary_expression();
	colt_base 			*unary_expressions();

	colt_isect			*isect();
	colt_union			*unionx();
	colt_complement		*complement();
	colt_base 			*file_name();
	colt_base 			*object_expression();

	colt_base 			*parse();
	colt_base			*parse_unary_expressions(colt_base *in);
};

#endif /* COLTPARSER_H_ */
