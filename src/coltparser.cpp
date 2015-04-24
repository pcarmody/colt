/*
 * coltparser.cpp
 *
 *  Created on: Jul 23, 2014
 *      Author: paul
 */
#include "colt_headers.h"

colt_parser::colt_parser(char *x, colt_datatype **p, char **heads, int num):
	input_buffer(),
	return_value(NULL),
	numfrags(0),
	cells(p),
	col_headers(heads),
	num_cols(num)
{
	// TODO Auto-generated constructor stub
	if(!cells) {
		input_buffer = new char[strlen(x)+1];
		strcpy(input_buffer, x);
	} else
		input_buffer = replace_string(x);
}

colt_parser::~colt_parser()
{
//	if(return_value)
//		delete return_value;
//	if(input_buffer && input_buffer != compiled_string)
//		delete input_buffer;
}

int colt_parser::is_a(Colt_Class c)
{
	COLT_TRACE("colt_parser::is_a(Colt_Class c)")
	return c == colt_class_parser;
}

int colt_parser::find_insertions(char **cols)
{
	COLT_TRACE("colt_parser::find_insertions(char **cols)")
	substrings[numfrags].index = -1;
	substrings[numfrags].substring[0] = '\0';
	for(char *a=input_buffer; *a; a++) {
		if(*a == '%') {
			for(int j=0; j<5; j++) {
				int col_len = strlen(cols[j]);
				if(strncmp(a+1, cols[j], col_len) == 0) {
					a += col_len;
					substrings[numfrags].index = j;
					break;
				}
			}
			numfrags++;
			substrings[numfrags].index = -1;
		}  else if (strncmp(a, "&{", 2) == 0) {
			for (int j=0; j<5; j++) {
				int col_len = strlen(cols[j]);
				if(strncmp(a+2, cols[j], col_len) == 0 && a[col_len+2] == '}') {
					a += col_len+2;
					substrings[numfrags].index = j;
					numfrags++;
					substrings[numfrags].index = -1;
					substrings[numfrags].substring[0] = '\0';
					break;
				}
			}
		} else {
			char *str = substrings[numfrags].substring;
			str[strlen(str)] = *a;
			str[strlen(str)+1] = '\0';
		}
	}

	return numfrags;
}

char *colt_parser::replace_strings(char **vals)
{
	COLT_TRACE("*colt_parser::replace_strings(char **vals)")
	compiled_string[0] = '\0';

	for(int i=0; i<numfrags+1; i++) {
		strcat(compiled_string, substrings[i].substring);
		if(substrings[i].index >= 0) {
			strcat(compiled_string, vals[substrings[i].index]);
		}
	}

	return compiled_string;
}

char *colt_parser::replace_string(char *in)
{
	COLT_TRACE("colt_parser::replace_string(char *)")
	char *b = compiled_string;

	for(char *a=in; *a; a++) {
		if (strncmp(a, "&{", 2) == 0) {
			for (int j=0; j<num_cols; j++) {
				int col_len = strlen(col_headers[j]);
				if(strncmp(a+2, col_headers[j], col_len) == 0 && a[col_len+2] == '}') {
					a += col_len+2;
					int len = cells[j]->format(b);
					b += len;
					break;
					break;
				}
			}
		} else if(*a == '%') {
			for(int j=0; j<num_cols; j++) {
				int col_len = strlen(col_headers[j]);
				if(strncmp(a+1, col_headers[j], col_len) == 0) {
					a += col_len;
					int len = cells[j]->format(b);
					b += len;
					break;
				}
			}
		}  else  {
			*b++ = *a;
		}
	}

	*b = '\0';

	return compiled_string;
}

//char *colt_parser::replace_strings(colt_datatypes **types)
//{
//	compiled_string[0] = '\0';
//
//	for(int i=0; i<numfrags+1; i++) {
//		strcat(compiled_string, substrings[i].substring);
//		if(substrings[i].index >= 0) {
//			char tmp[COLT_MAX_STRING_SIZE];
//			strcat(compiled_string, types[substrings[i].index]->format(tmp));
//		}
//	}
//}

void colt_parser::fatal_error(char const *err)
{
	COLT_TRACE("colt_parser::fatal_error(char const *err)")
	cerr<< err;
	exit(-1);
}

int colt_parser::is_token(const char *token)
{
//	COLT_TRACE("colt_parser::is_token(const char *token)")
	return strncmp(input_buffer, token, strlen(token)) == 0;
}

int colt_parser::consume_whitespace()
{
	COLT_TRACE("colt_parser::consume_whitespace()")
	char *b = input_buffer;

	while(*b && *b == '\t' || *b == ' ' || *b == '\n') b++;

	input_buffer = b;
	return 1;
}

char *colt_parser::consume_keyword(char *outbuff)
{
	COLT_TRACE("*colt_parser::consume_keyword(char *outbuff)")
	char *b=input_buffer;
	char *a = outbuff;
	char quote_char = '\0';

	if(*b == '"' || *b == '\'') {
		quote_char = *b++;
		while(*b && *b != quote_char)
			*a++ = *b++;
		*a = '\0';
		b++;
	} else {
//		while(*b && *b != '\t' && *b != ' ' && *b != '\n')
		while(isalnum(*b) || *b == '_' || *b == '-' || *b == '/' || *b == '.' || *b == '^')
			*a++ = *b++;
		*a = '\0';
	}

	input_buffer = b;

	consume_whitespace();

	return outbuff;
}

int colt_parser::consume_code_segment(char *outbuff)
{
	COLT_TRACE("colt_parser::consume_code_segment(char *outbuff)")
	char *b=input_buffer;
	int indent_level = 0;

	if(*b != '{') {
		cerr << "Expected a code segment at '" << input_buffer << "'.\n";
		exit(-1);
	}

	char *a = outbuff;
	b++;
	while(*b) {
		if(indent_level == 0 && *b == '}')
			break;
		*a++ = *b++;
		if(*b == '{')
			indent_level++;
	}

	*a = '\0';
	b++;
	input_buffer = b;

	return 1;
}

int colt_parser::consume_token(char *a)
{
	COLT_TRACE("colt_parser::consume_token(char *a)")
	char *b = input_buffer;

	if(strncmp(b, a, strlen(a)) != 0) {
		return 0;
	}

	input_buffer = b+strlen(a);
	consume_whitespace();
	return 1;
}

int colt_parser::consume_integer()
{
	COLT_TRACE("colt_parser::consume_integer()")
	if(!isdigit(*input_buffer))
		fatal_error("Expected an integer.\n");

	int retval = atoi(input_buffer);
	while(*input_buffer == '-' || (*input_buffer >= '0' && *input_buffer <= '9')) input_buffer++;

	return retval;
}

int colt_parser::consume_word(char *out)
{
	COLT_TRACE("colt_parser::consume_word(char *out)")
	char *a = input_buffer;

	while(isalnum(*a)
		|| *a == '.'
		|| *a == '_') *out++ = *a++;

	input_buffer = a;
	*out = '\0';
	 return 1;
}

int colt_parser::consume_colt_expression(char *out)
{
	COLT_TRACE("colt_parser::consume_colt_expression(char *out)")
	if(!consume_token("["))
		return 0;

	char *b = input_buffer;
	char exp[COLT_MAX_STRING_SIZE];
	char *a = out;
	int nest_count = 0;
	while(*b && (nest_count || *b != ']')) {
		if(*b == '\\' && *(b+1) == '[' )
			nest_count--;
		else if(*b == '[')
			nest_count++;
		*a++ = *b++;
	}
	*a = '\0';
	input_buffer = b+1;

	return 1;
}

colt_counter *colt_parser::count()
{
	COLT_TRACE("*colt_parser::count()")
	consume_token("count");
	return new colt_counter(*return_value);
}

colt_out_vertical *colt_parser::vertical()
{
	COLT_TRACE("*colt_parser::vertical()")
	consume_token("vertical");
	return new colt_out_vertical(*return_value);
}

colt_html *colt_parser::html()
{
	COLT_TRACE("*colt_parser::html()")
	consume_token("html");
	return new colt_html(*return_value);
}

colt_to_json *colt_parser::json()
{
	COLT_TRACE("*colt_parser::json()")
	consume_token("json");

	if(is_token(":")) {
		consume_token(":");
		char key[COLT_MAX_STRING_SIZE];

		consume_keyword(key);
		return new colt_to_json(*return_value, key);
	}

	return new colt_to_json(*return_value, NULL);
}

colt_out *colt_parser::csv()
{
	COLT_TRACE("*colt_parser::csv()")
	char col_sep = ',';
	char line_sep = '\n';
	char quote = '\0';

	consume_token("csv");

	if(consume_token(":")) {
		col_sep = *input_buffer++;
		if(consume_token(":")) {
			line_sep = *input_buffer++;
			if(consume_token(":"))
				quote = *input_buffer++;
		}
	}

	return new colt_out(*return_value, &col_sep, &line_sep, &quote);
}

colt_out_cbf *colt_parser::cbf()
{
	COLT_TRACE("*colt_parser::cbf()")
	colt_out_cbf *retval;
	char *in = input_buffer;

	if(in[3] != ':')
		retval = new colt_out_cbf(*return_value);
	else
		retval = new colt_out_cbf(*return_value, in+4);

	while(*in != ' ' && *in != '\t' && *in != '\n') in++;

	input_buffer = in;

	return retval;
}

colt_ask *colt_parser::ask()
{
	COLT_TRACE("*colt_parser:colt_ask()")
	consume_token("ask:");

	char hostname[COLT_MAX_STRING_SIZE];
	char port[COLT_MAX_STRING_SIZE];
	char expression[COLT_MAX_STRING_SIZE];

	consume_keyword(hostname);

	if(!consume_token(":"))
		fatal_error("'ask' expected ask:host:port,[colt expression]' missing colon.\n");

	consume_keyword(port);

	if(!consume_token(","))
		fatal_error("'ask' expected ask:host:port,[colt expression]' missing comma.\n");

	if(!consume_colt_expression(expression))
		fatal_error("'ask' expected ask:host:port,[colt expression]' missing expression.\n");

	return new colt_ask(expression, hostname, port);
}

colt_tell *colt_parser::tell()
{
	COLT_TRACE("*colt_parser::tell()")
	consume_token("tell:");

	int sockid = consume_integer();

	return new colt_tell(*return_value, sockid);
}

colt_base *colt_parser::file_name()
{
	COLT_TRACE("*colt_parser::file_name()")
	char file_name[COLT_MAX_STRING_SIZE];
	char dtype_str[COLT_MAX_STRING_SIZE];
	char col_sep='\0', eol_sep='\0', quote_sep='\0';

	consume_keyword(file_name);

	if(file_name[0] == '^') {
		if(!cells)
			fatal_error("Cannot parse '^' column reference.\n");

		for(int j=0; j<num_cols; j++)
			if(strcmp(file_name+1, col_headers[j] ) == 0)
				return (colt_base *) cells[j]->get_value();

		fatal_error("There is no column matching the column reference.\n");
	}

	if(consume_token(":")) {
		col_sep = *input_buffer++;
		if(consume_token(",")) {
			quote_sep = *input_buffer++;
			if(consume_token(","))
				eol_sep = *input_buffer++;
		}
	}

	if(is_token("[")) {
		consume_colt_expression(dtype_str);
//		if(!consume_token("]"))
//			fatal_error("Expected closing ']' in file datatype definiiton.\n");
	} else
		dtype_str[0] = '\0';

	colt_csv *retval = NULL;

	char *a = file_name;
	if(*a == '-')
		if(col_sep)
			retval = new colt_sequential_csv(a+1, col_sep, eol_sep, quote_sep);
		else
			retval = new colt_sequential_csv(a+1, 1);
	else if(match(file_name, "thru$"))
		return colt_load_thru(file_name);
	else if(col_sep)
			retval = new colt_csv(file_name, col_sep, eol_sep, quote_sep);
		else
			retval = new colt_csv(file_name, 0);

	retval->open_and_load();

	if(dtype_str[0])
		for(int i=0; i<strlen(dtype_str); i++)
			switch (dtype_str[i]) {
			case 'I':
				retval->set_datatype(i, COLT_DT_INTEGER);
				break;
			case 'D':
				retval->set_datatype(i, COLT_DT_SOURCE);
				break;
			}
	return retval;
}

colt_metadata *colt_parser::meta()
{
	COLT_TRACE("colt_parser::meta()")
	
	consume_token("meta");

	return new colt_metadata(*return_value);
}

colt_select *colt_parser::select()
{
	COLT_TRACE("*colt_parser::select()")
	int nom = !consume_token("select:") && consume_token("ignore:");

	int num_cols = 0;
	char buffer[COLT_MAX_STRING_SIZE];
	char *a = buffer;
	char *b = input_buffer;

	while(*b && *b!=' ' && *b!='\t' && *b != '\n' && *b != ';') *a++ = *b++;
	*a = '\0';
	input_buffer = b;

	char *col_list[100];

	b = buffer;
	col_list[num_cols++] = b;
	while(*b++)
		if(*b == ',') {
			*b = '\0';
			col_list[num_cols++] = ++b;
		}

	colt_select *retval = new colt_select(*return_value, col_list, num_cols, nom);

	return retval;
}


colt_skip_limit *colt_parser::skip_limit()
{
	COLT_TRACE("*colt_parser::skip_limit()")
	consume_token("limit:");
	int limit = consume_integer();
	int skip = 0;

	if(consume_token(",")) {
		skip = consume_integer();
	}

	colt_skip_limit *retval= new colt_skip_limit(*return_value, limit, skip);

	return retval;
}

colt_index *colt_parser::index()
{
	COLT_TRACE("*colt_parser::index()")
	consume_token("index:");
	int list[1000];
	int count = 0;

	list[count++] = consume_integer();
	while(consume_token(","))
		list[count++] = consume_integer();

	return new colt_index(*return_value, count, list);
}

colt_if *colt_parser::ifx()
{
	COLT_TRACE("*colt_parser::ifx()")
	char in[1000];

	consume_token("if:");

	if(*input_buffer == '{')
		consume_code_segment(in);
	else {
		char lookup_key[COLT_MAX_STRING_SIZE];
		consume_keyword(lookup_key);
		consume_token(",");
		consume_code_segment(in);
		return new colt_if(*return_value, in, lookup_key);
	}

	return new colt_if(*return_value, in);
}

colt_add *colt_parser::add()
{
	COLT_TRACE("*colt_parser::add()")
	char repl_str[COLT_MAX_STRING_SIZE];
	char label[COLT_MAX_STRING_SIZE];
	char type_str[COLT_MAX_STRING_SIZE];

	consume_token("add:");

	consume_word(label);

	if(!is_token(","))
		fatal_error("Expected add:label,type,value.  Comma missing after label.\n");

	consume_token(",");

	consume_word(type_str);

	if(!is_token(","))
		fatal_error("Expected add:label,type,value.  Comma missing after type.\n");

	consume_token(",");

	int type = 0;

	if(strcmp(type_str, "source") == 0) {

		type = COLT_DT_SOURCE;
		if(!consume_colt_expression(repl_str))
			fatal_error("add expression expected a [colt-expression].\n");

		return new colt_add_source(*return_value, label, type, repl_str);

	} else if(strcmp(type_str, "bash") == 0) {

		type = COLT_DATATYPE;
		if(!consume_colt_expression(repl_str))
			fatal_error("add expression expected a [bash-expression].\n");

		return new colt_bash(*return_value, label, type, repl_str);

	} else {

		if(strcmp(type_str, "string") == 0)
			type = COLT_DATATYPE;
		else if(strcmp(type_str, "int") == 0)
			type = COLT_DT_INTEGER;
	//		else if(strcmp(type_str, "date") == 0)
	//			type = COLT_DT_DATE;
		else
			fatal_error("Expected datatype in add expression.\n");

		if(is_token("{")) {
			consume_code_segment(repl_str);

			return new colt_cexpression(*return_value, label, type, repl_str);
		} else
			consume_keyword(repl_str);

	}

	return new colt_add(*return_value, label, type, repl_str);
}

colt_link *colt_parser::link()
{
	COLT_TRACE("*colt_parser::link()")
	consume_token("link:");

	char col_name[COLT_MAX_STRING_SIZE];
	consume_word(col_name);

	if(!consume_token(","))
		fatal_error("Link syntax error expected ',' after column name.\n");

//	if(consume_token("index"))
//		coltype = COLT_DT_INDEX;
//	else if(consume_token("list"))
//		coltype = COLT_DT_INDEX_LIST;
//	else if(consume_token("range"))
//		coltype = COLT_DT_RANGE;
//	else if(consume_token("bitmap"))
//		coltype = COLT_DT_BITMAP;
//	else
//		fatal_error("Link expected one of 'index', 'list', 'range', or 'bitmap'.\n");

	if(!consume_token(","))
		fatal_error("Link syntax error expected ',' after link type.\n");

	char filename[COLT_MAX_STRING_SIZE];
	consume_word(filename);

	char exp[COLT_MAX_STRING_SIZE];
	consume_colt_expression(exp);

	if(!consume_token(","))
		fatal_error("Link syntax error expected ',' after file name.\n");

	return new colt_link(*return_value, col_name, exp);
}

colt_aggregate *colt_parser::aggregate()
{
	COLT_TRACE("*colt_parser::aggregate()")
	char *b=input_buffer+3;

	if(*b != ':')
		fatal_error("Expected aggregation expression after agg.\n");

	colt_aggregate *retval = new colt_aggregate(*return_value);

	do {
		char function[10];
		char *a=function;
		b++;
		for(int i=0; i<10 && *b && *b >= 'a' && *b <= 'z'; i++)
			*a++ = *b++;
		*a = '\0';

		int func_code;
		if(strcmp(function, "sum") == 0)
			func_code = colt_aggregate::sum;
		else if(strcmp(function, "count") == 0)
			func_code = colt_aggregate::count;
		else if(strcmp(function, "max") == 0)
			func_code = colt_aggregate::max;
		else if(strcmp(function, "min") == 0)
			func_code = colt_aggregate::min;
		else if(strcmp(function, "first") == 0)
			func_code = colt_aggregate::first;
		else if(strcmp(function, "last") == 0)
			func_code = colt_aggregate::last;
		else
			fatal_error("Expected one of 'count', 'sum', 'first', 'last', 'max', 'min' in agg.\n");

		if(*b != '=')
			fatal_error("Expected an '=' with the aggregate function.\n");

		char col_name[100];
		a = col_name;
		b++;
		while(*b && *b != ' ' && *b != ',' && *b != '\n') *a++ = *b++;
		*a = '\0';

		retval->add_aggregation(func_code, col_name);
	} while (*b == ',');

	input_buffer = b;

	return retval;
}

colt_aggregate_row *colt_parser::aggregate_row()
{
	COLT_TRACE("*colt_parser::aggregate_row()")
	int func_code;
	consume_token("aggrow:");

	if(consume_token("first"))
		func_code = colt_aggregate::first;
	else if(consume_token("last"))
		func_code = colt_aggregate::last;
	else if(consume_token("max"))
		func_code = colt_aggregate::max;
	else if(consume_token("min"))
		func_code = colt_aggregate::min;
	else
		fatal_error("Expected one of 'first', 'last', 'max', 'min' in aggrow.\n");

	int count = 1;
	char col_name[COLT_MAX_STRING_SIZE];
	if(consume_token("="))
		consume_keyword(col_name);
	else
		fatal_error("Expected '=column name' in aggrow.\n");

	if(consume_token(","))
		count = consume_integer();

	colt_aggregate_row *retval = NULL;

	retval = new colt_aggregate_row(*return_value, func_code, count, col_name);

	return retval;
}

colt_sort *colt_parser::sort()
{
	COLT_TRACE("*colt_parser::sort()")
	char key[100];
	int  type=0;
	int  direction=1;
	char *b=input_buffer;
	char file_name[COLT_MAX_STRING_SIZE];

	if(b[4] == ':') {
		char *a=key;
		b += 5;
		while(*b && *b != ',' && *b != ' ') *a++ = *b++;
		*a = '\0';
	}

	if(*b == ',') {
		b++;
		if(*b != 'n' && *b != 'a') {
			perror("sort type parameter expected either 'a' for alpha or'n' numeric.\n");
			exit(-1);
		}
		type = (*b++ == 'n');
	}

	if(*b == ',') {
		b++;
		if(*b != 'd' && *b != 'a') {
			perror("sort asscending parameter expected either 'a' for ascending or'd' for descending.\n");
			exit(-1);
		}
		direction = (*b++ == 'a');
	}

	colt_sort *retval = NULL;

	if(*b == ':') {
		char *a=file_name;
		b++;
		while(*b && *b != ',' && *b != ':' && *b != ' ') *a++ = *b++;
		*a = '\0';
		retval = new colt_sort(*return_value, file_name, key, type, direction);
	} else
		retval = new colt_sort(*return_value, key, type, direction);

	while(*b && *b != ' ' && *b != '\t' && *b != '\n') b++;

//	return_value = retval;

	input_buffer = b;

	return retval;
}

coltthru *colt_parser::thru()
{
	COLT_TRACE("*colt_parser::thru()")
	char file_name[COLT_MAX_STRING_SIZE];

	consume_token("thru");

	coltthru *retval = NULL;

	if(consume_token(":")) {
		if(consume_keyword(file_name))
			retval = new coltthru(*return_value, file_name);
		else
			fatal_error("Unable to parse file_name in 'thru'.\n");
	} else
		retval = new coltthru(*return_value);

	return retval;
}

colt_keyspace *colt_parser::keyspace()
{
	COLT_TRACE("*colt_parser::keyspace()")
	input_buffer += 8;

	char *b=input_buffer;
	char file_name[COLT_MAX_STRING_SIZE];

	if(!return_value->is_a(colt_class_sort))
		fatal_error("keyspace must follow a sorted thru.\n");

	colt_keyspace *retval = NULL;
	int col = ((colt_sort *) return_value)->get_column_number();

	if(*b == ':') {
		char *a=file_name;
		b++;
		while(*b && *b != ',' && *b != ':' && *b != ' ') *a++ = *b++;
		*a = '\0';
		retval = new colt_keyspace(*return_value, col, file_name);
	} else
		retval = new colt_keyspace(*return_value, col, NULL);

	input_buffer = b;
//	return_value = retval;

	return retval;
}

colt_cthru *colt_parser::cthru()
{
	COLT_TRACE("*colt_parser::cthru()")
	char key[100];
	int  type=0;
	int  direction=1;
	char file_name[COLT_MAX_STRING_SIZE];

	file_name[0] = '\0';

	consume_token("cthru");

	if(!consume_token(":")
	|| !consume_word(key))
		fatal_error("cthru syntax is 'cthru:key<,a/n><,a/d>");

	if(consume_token(","))
		if(consume_token("d"))
			direction = 1;
		else if(!consume_token("a"))
			fatal_error("cthru syntax requires sort order as 'a' or 'd'.\n");

	if(consume_token(","))
		if(consume_token("n"))
			type = 1;
		else if(!consume_token("a"))
			fatal_error("chtru syntax requires sort type as 'a' or 'n'.\n");

	if(consume_token(":"))
		if(!consume_word(file_name))
			fatal_error("cthru expected a filename after ';'.\n");

	colt_cthru *retval = NULL;

	if(file_name[0])
		retval = new colt_cthru(*return_value, file_name, key, type, direction);
	else
		retval = new colt_cthru(*return_value, key, type, direction);

	return retval;
}

colt_range *colt_parser::range()
{
	COLT_TRACE("*colt_parser::range()")
	consume_token("range");

	char low[COLT_MAX_STRING_SIZE], high[COLT_MAX_STRING_SIZE];
	colt_range *retval = NULL;

	if(!consume_token(":"))
		fatal_error("range expected a ';'.\n");

	if(!consume_keyword(low))
		fatal_error("range expected a LOW,hig.\n");

	if(consume_token(","))
		if(consume_keyword(high))
			retval = new colt_range(*return_value, low, high);
		else
			fatal_error("range syntax is 'range:low,HIGH'.\n");
	else
		retval = new colt_range(*return_value, low, low);

	return retval;
}

coltbitmap *colt_parser::bitmap()
{
	COLT_TRACE("*colt_parser::bitmap()")

	consume_token("bitmap");
	coltbitmap *retval = NULL;

	if(consume_token(":")) {
		char file_name[COLT_MAX_STRING_SIZE];
		consume_keyword(file_name);
		retval = new coltbitmap(*return_value, file_name);
	} else
		retval = new coltbitmap(*return_value);

//	return_value = retval;
	return retval;
}

colt_each *colt_parser::each()
{
	COLT_TRACE("*colt_parser::each()")
	input_buffer += 4;

	if(*input_buffer != ':' &&
	   *input_buffer+1 != '[')
		fatal_error("Each expected a colt expression.\n");

	char *b = input_buffer+2;
	char exp[COLT_MAX_STRING_SIZE];
	char *a = exp;
	while(*b && *b != ']') *a++ = *b++;
	*a = '\0';
	input_buffer = b+1;
	colt_each *retval = new colt_each(*return_value, exp);

	return_value = retval;
}

colt_isin *colt_parser::isin()
{
	COLT_TRACE("*colt_parser::isin()")

	int negative = consume_token("notin");
	if(!negative)
		consume_token("isin");

	if(!consume_token(":"))
		fatal_error("isin/notin expected a ':'.\n");

	if(input_buffer[0] != '[')
		fatal_error("isin/notin expected a colt expression.\n");

	char command[COLT_MAX_STRING_SIZE];

	consume_colt_expression(command);

	return new colt_isin(*return_value, command, negative);
}

colt_cross *colt_parser::cross()
{
	COLT_TRACE("*colt_parser::cross()")

	consume_token("cross");

	if(*input_buffer != ':' &&
	   *input_buffer+1 != '[')
		fatal_error("'cross' expected a colt expression.\n");

	char *b = input_buffer+2;
	char exp[COLT_MAX_STRING_SIZE];
	char *a = exp;
	while(*b && *b != ']') *a++ = *b++;
	*a = '\0';
	input_buffer = b+1;
	return new colt_cross(*return_value, exp);
}

colt_race *colt_parser::race()
{
	COLT_TRACE("*colt_parser::cross()")

	consume_token("race");

	if(!consume_token(":"))
		fatal_error("Expected ':' after 'race'.\n");

	char col_name[COLT_MAX_STRING_SIZE];
	char left_name[COLT_MAX_STRING_SIZE];
	char right_name[COLT_MAX_STRING_SIZE];
	char command[COLT_MAX_STRING_SIZE];
	char flags[COLT_MAX_STRING_SIZE];

	if(input_buffer[0] == '0' || input_buffer[0] == '1') {
		consume_keyword(flags);

		if(!consume_token(","))
			fatal_error("Expected ',' after race:flags\n");
	} else
		strcpy(flags, "01");

	consume_keyword(col_name);

	if(!consume_token(","))
		fatal_error("Expected ',' after race:flags.\n");

	consume_keyword(left_name);

	if(!consume_token(","))
		fatal_error("Expected ',' after race:flags,col_name,left_key.\n");\

	consume_keyword(right_name);

	if(!consume_token(","))
		fatal_error("Expected ',' after race:flags,col_name,left_key,right_key.\n");

	if(*input_buffer != '[')
		fatal_error("'race' expected a colt expression.\n");

	consume_colt_expression(command);

	return new colt_race(*return_value, col_name, left_name, right_name, flags, command);
}

colt_partition *colt_parser::partition()
{
	COLT_TRACE("*colt_parser::partition()")
	input_buffer += 4;

	if(*input_buffer != ':')
		fatal_error("Partition expected a key and a colt expression.\n");

	char *b = input_buffer+1;
	char key[100];
	char *a=key;
	while(*b && *b != ',' && *b != ' ') *a++ = *b++;
	*a = '\0';

	if(*b != ',')
		fatal_error("Expected comma after key in part expression.\n");

	b += 2;

	char exp[COLT_MAX_STRING_SIZE];
	a = exp;
	while(*b && *b != ']') *a++ = *b++;
	*a = '\0';
	input_buffer = b+1;

	colt_partition *retval = new colt_partition(*return_value, exp, key);

	return retval;
}

colt_expand *colt_parser::expand()
{
	COLT_TRACE("*colt_parser::expand()")
	consume_token("expand");

	if(*input_buffer != ':')
		fatal_error("expand expected a key and a colt expression.\n");

	char *b = input_buffer+1;
	char key[100];
	char *a=key;
	while(*b && *b != ',' && *b != ' ') *a++ = *b++;
	*a = '\0';

	if(*b != ',')
		fatal_error("Expected comma after key in expand expression.\n");

	b += 2;

	char exp[COLT_MAX_STRING_SIZE];
	a = exp;
	while(*b && *b != ']') *a++ = *b++;
	*a = '\0';
	input_buffer = b+1;

	colt_expand *retval = new colt_expand(*return_value, key, exp);

	return retval;
}

colt_reduce *colt_parser::reduce()
{
	COLT_TRACE("*colt_parser::reduce()")
	consume_token("reduce");
	if(!consume_token(":"))
		return new colt_reduce(*return_value, 0);

	if(isdigit(*input_buffer))
		return new colt_reduce(*return_value, consume_integer());

	char filename[COLT_MAX_STRING_SIZE];
	if(!consume_word(filename))
		fatal_error("Reduce expression expected an integer or a datasource.\n");

	return new colt_reduce(*return_value, filename);
}

colt_onchange *colt_parser::onchange()
{
	COLT_TRACE("*colt_parser::onchange()")
	consume_token("onchange:");

	char *b = input_buffer;
	char key[100];
	char *a=key;
	while(*b && *b != ',' && *b != ' ') *a++ = *b++;
	*a = '\0';

	if(*b != ',')
		fatal_error("Expected comma after key in onchange expression.\n");

	b += 2;

	char exp[COLT_MAX_STRING_SIZE];
	a = exp;
	while(*b && *b != ']') *a++ = *b++;
	*a = '\0';
	input_buffer = b+1;

	colt_onchange *retval = new colt_onchange(*return_value, exp, key);

	return retval;
}

colt_uniq *colt_parser::uniq()
{
	COLT_TRACE("colt_parser::uniq()")
	consume_token("uniq:");

	char col_name[COLT_MAX_STRING_SIZE];
	int last=0;

	consume_keyword(col_name);

	if(consume_token("!"))
		last = 1;

	return new colt_uniq(*return_value, col_name, last);
}

colt_share *colt_parser::share()
{
	COLT_TRACE("*colt_parser::share()")
	consume_token("share:[");

	colt_share *retval = new colt_share(*return_value);
	colt_base *tmp_return_value = return_value;

	char *b = input_buffer;
	char expression[COLT_MAX_STRING_SIZE];
	char *a = expression;

	while(*b && *b != ']') *a++ = *b++;
	*a = '\0';
	input_buffer = b;
	colt_parser local_buffer(expression);
	do {
		colt_operator *exp = new colt_operator;
		local_buffer.parse_unary_expressions(exp);
		local_buffer.consume_whitespace();
		retval->add_expression(exp);
	} while(local_buffer.consume_token(";"));

	return_value = tmp_return_value;

	consume_whitespace();

	consume_token("]");

	return retval;
}

colt_thread *colt_parser::threadx()
{
	COLT_TRACE("*colt_parser::threadx()")
	consume_token("thread");
	return new colt_thread(*return_value);
}

colt_sync *colt_parser::sync()
{
	COLT_TRACE("*colt_parser::sync()")
	consume_token("sync");
	return new colt_sync(*return_value);
}

colt_base *colt_parser::unary_expression()
{
	COLT_TRACE("*colt_parser::unary_expression()")
	colt_base *object = NULL;

	if(is_token("select") || is_token("ignore"))
		object = select();
	else if(is_token("limit"))
		object = skip_limit();
	else if(is_token("index"))
		object = index();
	else if(is_token("if"))
		object = ifx();
	else if(is_token("sort"))
		object = sort();
	else if(is_token("keyspace"))
		object = keyspace();
	else if(is_token("cthru"))
		object = cthru();
	else if(is_token("range"))
		object = range();
	else if(is_token("thru"))
		object = thru();
	else if(is_token("bitmap"))
		object = bitmap();
	else if(is_token("each"))
		object = each();
	else if(is_token("isin"))
		object = isin();
	else if(is_token("notin"))
		object = isin();
	else if(is_token("cross"))
		object = cross();
	else if(is_token("race"))
		object = race();
	else if(is_token("part"))
		object = partition();
	else if(is_token("expand"))
		object = expand();
	else if(is_token("onchange"))
		object = onchange();
	else if(is_token("uniq"))
		object = uniq();
	else if(is_token("aggrow"))
		object = aggregate_row();
	else if(is_token("agg"))
		object = aggregate();
	else if(is_token("share"))
		object = share();
	else if(is_token("thread"))
		object = threadx();
	else if(is_token("sync"))
		object = sync();
	else if(is_token("add"))
		object = add();
	else if(is_token("link"))
		object = link();
	else if(is_token("reduce"))
		object = reduce();
	else if(is_token("meta"))
		object = meta();

	consume_whitespace();

	return object;
}

colt_base *colt_parser::unary_expressions()
{
	COLT_TRACE("*colt_parser::unary_expressions()")
	colt_base *retval = NULL;
	colt_base *object = NULL;

	while(object = unary_expression()) {
		if(!retval)
				retval = object;	// return only the first of the object chain.
	}

	return retval;
}

colt_base *colt_parser::output_expression()
{
	COLT_TRACE("*colt_parser::output_expression()")
	if(is_token("count"))
		return count();

	if(is_token("vertical"))
		return vertical();

	if(is_token("html"))
		return html();

	if(is_token("json"))
		return json();

	if(is_token("csv"))
		return csv();

	if(is_token("cbf"))
		return cbf();

	if(is_token("tell"))
		return tell();

	return new colt_out(*return_value, "\t");
}

colt_isect *colt_parser::isect()
{
	COLT_TRACE("*colt_parser::isect()")
	input_buffer += 5;
	if(*input_buffer != ':')
		fatal_error("Expected isect:file,file\n");

	input_buffer++;
	colt_base *left = file_name();

	if(*input_buffer != ',')
		fatal_error("isect expected second parameter.\n");

	input_buffer++;
	colt_base *right = file_name();

	return new colt_isect(*left, right);
}

colt_union *colt_parser::unionx()
{
	COLT_TRACE("*colt_parser::unionx()")
	input_buffer += 5;
	if(*input_buffer != ':')
		fatal_error("Expected union:file,file\n");

	input_buffer++;
	colt_base *left = file_name();

	if(*input_buffer != ',')
		fatal_error("union expected second parameter.\n");

	input_buffer++;
	colt_base *right = file_name();

	return new colt_union(*left, right);
}

colt_complement *colt_parser::complement()
{
	COLT_TRACE("*colt_parser::complement()")
	input_buffer += 5;
	if(*input_buffer != ':')
		fatal_error("Expected complement:file,file\n");

	input_buffer++;
	colt_base *left = file_name();

	if(*input_buffer != ',')
		fatal_error("complement expected second parameter.\n");

	input_buffer++;
	colt_base *right = file_name();

	if(!right->is_a(colt_class_bitmap))
		fatal_error("complement right-operand must be a bitmap.\n");

	return new colt_complement(*left, (coltbitmap *) right);
}

colt_base *colt_parser::object_expression()
{
	COLT_TRACE("*colt_parser::object_expression()")

	if(is_token("isect"))
		return_value = isect();
	else if(is_token("union"))
		return_value = unionx();
	else if(is_token("comp"))
		return_value = complement();
	else if(is_token("ask"))
		return_value = ask();
	else
		return_value = file_name();

	return return_value;
}

colt_base *colt_parser::parse(int no_default_out)
{
	COLT_TRACE("*colt_parser::parse()")
//	input_buffer = input;

	return_value = object_expression();

	consume_whitespace();

	unary_expressions();

	consume_whitespace();

	if(!no_default_out)
		output_expression();
	else
		if(*input_buffer)
			output_expression();

	consume_whitespace();

	if(*input_buffer) {
		cerr << "Parsing error at '" << input_buffer << "'\n";
		exit(-1);
	}

	return return_value;
}


colt_base *colt_parser::parse_unary_expressions(colt_base *in)
{
	COLT_TRACE("*colt_parser::parse_unary_expressions(colt_base *in)")
	return_value = in;

	colt_base *retval =  unary_expressions();

	if(retval)
		return retval;

	return output_expression();
//	return in;
}
