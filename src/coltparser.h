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
	struct StrFragment substrings[10];
	int 		numfrags;
	char		compiled_string[COLT_MAX_STRING_SIZE];
	colt_datatype	**cells;
	char			**col_headers;
	int				num_cols;

	colt_parser(char *, colt_datatype **p=NULL, char **heads=NULL, int num=0);
	virtual ~colt_parser();

	int find_insertions(char **cols);
	char *replace_strings(char **vals);
	char *replace_string(char *in);
//	char *replace_strings(colt_datatype **types);

	int is_a(Colt_Class c);
	void fatal_error(char const *err);
	int consume_whitespace();
	int consume_word(char *t);
	int consume_colt_expression(char *out);
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
	colt_out_cbf		*cbf();
	colt_yml			*yml();
	colt_ask			*ask();
	colt_tell			*tell();
	colt_base			*output_expression();

	colt_metadata		*meta();
	colt_select 		*select();
	colt_skip_limit		*skip_limit();
	colt_index			*index();
	colt_if				*ifx();
	colt_add			*add();
	colt_link			*link();
	colt_aggregate		*aggregate();
	colt_aggregate_row	*aggregate_row();
	colt_sort			*sort();
	coltthru			*thru();
	colt_keyspace		*keyspace();
	colt_cthru			*cthru();
	colt_range			*range();
	coltbitmap			*bitmap();
	colt_qthru			*qthru();
	colt_each			*each();
	colt_isin			*isin();
	colt_cross			*cross();
	colt_race			*race();
	colt_partition		*partition();
	colt_pthru			*pthru();
	colt_expand			*expand();
	colt_reduce			*reduce();
	colt_onchange		*onchange();
	colt_uniq			*uniq();
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

	colt_base 			*parse(int no_default_out=0);
	colt_base			*parse_unary_expressions(colt_base *in);
};

#endif /* COLTPARSER_H_ */
