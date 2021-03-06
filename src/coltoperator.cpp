/*
 * coltoperator.cpp
 *
 *  Created on: May 7, 2014
 *      Author: paulcarmody
 */
#include "colt_headers.h"

colt_operator::colt_operator(colt_base &in)
{
	operand = &in;
	in.set_destination(this);
}

colt_operator::colt_operator():
		colt_base(),
		operand(NULL)
{}

colt_operator::~colt_operator()
{
	// TODO Auto-generated destructor stub
}

int colt_operator::is_a(Colt_Class c)
{
	COLT_TRACE("colt_operator::is_a(Colt_Class c)")
	return c == i_am || colt_base::is_a(c);
}

void colt_operator::set_destination(colt_base *obj, int bothways)
{
	COLT_TRACE("colt_operator::set_destination(colt_base *obj, int bothways)")
//	colt_base::set_destination(obj);
	if(out_object)
		return out_object->set_destination(obj, bothways);

	out_object = obj;
	if(bothways) {
//		out_object = obj;
		obj->set_operand(this);
	}
}

char *colt_operator::source_file_name()
{
	COLT_TRACE("*colt_operator::source_file_name()");
	if(!operand)
		return "";
	return operand->source_file_name();
}

colt_base *colt_operator::get_datasource(int count)
{
	COLT_TRACE("*colt_operator::get_datasource(int count)")
	return operand->get_datasource(count);
}

int colt_operator::reduce_to(int level, int rec_num)
{
	COLT_TRACE("colt_operator::reduce_to(int level, int rec_num)")
	return operand->reduce_to(level, rec_num);
}

void colt_operator::set_operand(colt_base *op)
{
	COLT_TRACE("colt_operator::set_operand(colt_base *op)")
	operand = op;
}

char *colt_operator::record(int line_num)
{
	COLT_TRACE("*colt_operator::record(int line_num)")
	if(!operand)
		return NULL;

	return operand->record(line_num);
}

char *colt_operator::col_header(int n)
{
	COLT_TRACE("*colt_operator::col_header(int n)")
	if(!operand)
		throw "colt_operator::col_header unintialized operand.\n";
	return operand->col_header(n);
}

char **colt_operator::col_headers()
{
	COLT_TRACE("*colt_operator::col_headers()")
	if(!operand)
		throw "colt_operator::col_headers unintialized operand.\n";
	return operand->col_headers();
}

char *colt_operator::field_val(int rec_num, int col_num)
{
	COLT_TRACE("*colt_operator::field_val(int rec_num, int col_num)")
	if(!operand)
		return NULL;

	return operand->field_val(rec_num, col_num);
}

char *colt_operator::extract_str(char *in, char sep_char, char eol_char)
{
	COLT_TRACE("*colt_operator::extract_str(char *in, char sep_char, char eol_char)")
	if(operand)
		return operand->extract_str(in, sep_char, eol_char);
	return NULL;
}

char colt_operator::sep_char()
{
	COLT_TRACE("colt_operator::sep_char()")
	return operand->sep_char();
}

int colt_operator::num_lines()
{
	COLT_TRACE("colt_operator::num_lines()")
	return operand->num_lines();
}

int colt_operator::max_size()
{
	COLT_TRACE("colt_operator::max_size()")
	return operand->max_size();
}

int colt_operator::num_cols()
{
	COLT_TRACE("colt_operator::num_cols()")
	return operand->num_cols();
}

int colt_operator::compare(int a, int b, int c)
{
	COLT_TRACE("colt_add::compare(int a, int b, int c)")
	return operand->compare(a,b,c);
}

int colt_operator::contains(int rec_num)
{
	return operand && operand->contains(rec_num);
}

char *colt_operator::index_file_name()
{
	if(!operand)
		return NULL;
	return operand->index_file_name();
}

char **colt_operator::fields(int rec_num)
{
	COLT_TRACE("**colt_operator::fields(int rec_num)")
	return operand->fields(rec_num);
}

void colt_operator::set_coltype(int num, colt_datatype *x)
{
	COLT_TRACE("colt_operator::set_coltype(int num, colt_datatype *x)")
	return operand->set_coltype(num,  x);
}

colt_datatype **colt_operator::cells(int rec_num)
{
	COLT_TRACE("**colt_operator::cells(int rec_num)")
	return operand->cells(rec_num);
}

colt_nested_cells *colt_operator::nested_cells(int rec_num)
{
	COLT_TRACE("**colt_operator::nested_cells(int rec_num)")
	return operand->nested_cells(rec_num);
}

int colt_operator::get_next_row()
{
	COLT_TRACE("colt_operator::get_next_row()")
	return operand->get_next_row();
}
