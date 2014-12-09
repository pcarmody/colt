/*
 * coltbase.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"

//
// colt_base
//

char	colt_base::extraction_string[10000];

colt_base::colt_base():
	out_object(NULL),
	i_am(colt_class_base)
{
	// TODO Auto-generated constructor stub

}

colt_base::~colt_base() {
	// TODO Auto-generated destructor stub
}

int colt_base::is_a(Colt_Class c)
{
	COLT_TRACE("colt_base::is_a(Colt_Class c)")
	return 0;
}

colt_base *colt_base::copy_expression(colt_base *op)
{
	COLT_TRACE("*colt_base::copy_expression(colt_base *op)")
	colt_base *retval = copy(op);

	if(out_object)
		retval->out_object = out_object->copy_expression(retval);

	return retval;
}

colt_base *colt_base::copy(colt_base *op)
{
	COLT_TRACE("*colt_base::copy(colt_base *op)")
	return new colt_base();
}

void colt_base::set_destination(colt_base *obj, int bothways)
{
	COLT_TRACE("colt_base::set_destination(colt_base *obj, int bothways)")
	if(out_object)
		return out_object->set_destination(obj);

	out_object = obj;

//	obj->set_operand(this);
}

colt_base *colt_base::get_destination()
{
	COLT_TRACE("*colt_base::get_destination()")
	if(out_object)
		return out_object->get_destination();

	return this;
}

void colt_base::set_operand(colt_base *b)
{
	COLT_TRACE("colt_base::set_operand(colt_base *b)")
	return;
}

char *colt_base::record(int line_num)
{
	COLT_TRACE("*colt_base::record(int line_num)")
	return NULL;
}

char *colt_base::col_header(int n)
{
	COLT_TRACE("*colt_base::col_header(int n)")
	return NULL;
}

char **colt_base::col_headers()
{
	COLT_TRACE("*colt_base::col_headers()")
	return NULL;
}

char *colt_base::field_val(int rec_num, int col_num)
{
	COLT_TRACE("*colt_base::field_val(int rec_num, int col_num)")
	return NULL;
}

int colt_base::num_lines() 
{ 
	COLT_TRACE("colt_base::num_lines() ")
	return 0;
};

int colt_base::num_cols() 
{ 
	COLT_TRACE("colt_base::num_cols() ")
	return 0; 
};

int colt_base::max_size() 
{ 
	COLT_TRACE("colt_base::max_size() ")
	return 0; 
};

char *colt_base::source_file_name() 
{ 
	COLT_TRACE("*colt_base::source_file_name() ")
	return NULL; 
}

colt_base *colt_base::get_datasource(int count) 
{ 
	COLT_TRACE("*colt_base::get_datasource(int count) ")
	return this; 
}

int colt_base::reduce_to(int level, int rec_num)
{
	COLT_TRACE("colt_base::reduce_to(int level, int rec_num)")
	return rec_num;
}

int colt_base::compare(int a, int b, int c)
{
	return a-b;
}

char *colt_base::extract_str(char *in, char sep_char, char eol_char, char quote_char) 
{
	COLT_TRACE("*colt_base::extract_str(char *in, char sep_char, char eol_char, char quote_char) ")
	return in;
//	extraction_string[0] = '\0';
//	if(in == NULL) {
//		return extraction_string;
//	} else if(*in == sep_char)
//		return extraction_string;
//
//	char *a = extraction_string;
//	int len = sizeof(extraction_string);
//	int i=0;
//
//	if(quote_char)
//		if(*in == quote_char)
//			in++;
//
//	for(char *b = in; *b != sep_char && *b != eol_char; b++) {
//		*a++ = *b;
//		if(i++ == len)
//			break;
//	}
//
//	if(quote_char && *(a-1) == quote_char)
//		*(a-1) = '\0';
//	else
//		*a = '\0';
//	return extraction_string;
}

char **colt_base::fields(int rec_num)
{
	COLT_TRACE("**colt_base::fields(int rec_num)")

	return NULL;
	//for(int col=0; col < num_cols; col++)
	//	field[col] = field_val(rec_num, col)
}

colt_datatype **colt_base::cells(int rec_num)
{
	COLT_TRACE("**colt_base::cells(int rec_num)")
	return NULL;
}

void colt_base::set_coltype(int num, colt_datatype *x)
{
	COLT_TRACE("colt_base::set_coltype(int num, colt_datatype *x)")

}

char colt_base::sep_char()
{
	COLT_TRACE("colt_base::sep_char()")
	return '\0';
}

char colt_base::find_sep_char(char *row, char end_char)
{
	COLT_TRACE("colt_base::find_sep_char(char *row, char end_char)")
	// space = 32,  / = 47 and 91 = [ thru ` = 96
	int count[127];
	for(int i=0; i<127; i++) count[i] = 0;
	for(; *row && *row != end_char; row++) {
		if((*row == '\t')
		|| (*row >= 35 && *row <= 47)
		|| (*row >= 58 && *row <= 64)
		|| (*row >= 91 && *row <= 95)
		|| (*row == '~')) {
			count[*row]++;
		}
	}
	int max_index = 0;
	for(int i=0; i<127; i++)
		if(count[max_index] < count[i])
			max_index = i;
	return (char) max_index;
}

int colt_base::get_next_row()
{
	COLT_TRACE("colt_base::get_next_row()")
	return -1;
}

int colt_base::preprocess()
{
	COLT_TRACE("colt_base::preprocess()")
	if(out_object)
		return out_object->preprocess();

	return 1;
}

void colt_base::postprocess()
{
	COLT_TRACE("colt_base::postprocess()")
	if(out_object)
		out_object->postprocess();
}

int colt_base::process(int rec)
{
	COLT_TRACE("colt_base::process(int rec)")
	if(out_object)
		return out_object->process(rec);

	return 1;
}

void colt_base::process_all()
{
	COLT_TRACE("colt_base::process_all()")
	int rec;
	if(!preprocess())
		return;

	try {
		while((rec = get_next_row()) >= 0) {
			process(rec);
		}
	} catch (colt_exception *e) {

	}

	postprocess();
}
