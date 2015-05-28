/*
 * coltnestedcells.cpp
 *
 *  Created on: May 20, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltnestedcells.h"

int test_colt_nested_cells()
{
	char *outer1[] = { "1962", "1977", "1989", "2010" };
	char *outer2[] = { "1990", "2001", "2008", "2015" };
	char *outer3[] = { "1993", "2001", "2009", "2013" };

	char *inner1[] = { "Jan", "Feb", "March" };
	char *inner2[] = { "Apr", "May", "June" };
	char *inner3[] = { "July", "August", "September" };
	char *inner4[] = { "October", "November", "December" };

	char *deep1[] = { "14", "9", "2" };
	char *deep2[] = { "25", "4", "0" };

	char *head1[] = { "Birth", "start", "better", "bests" };
	char *head2[] = { "Mon1", "mon2", "Month3" };
	char *head3[] = { "father", "daughter", "son" };

	colt_nested_cells deepest1(deep1, 3, head3, "days");
	colt_nested_cells deepest2(deep2, 3, head3, "days");

	colt_nested_cells in11(inner1, 3, head2, "Months", &deepest1);
	colt_nested_cells in12(inner1, 3, head2, "Months", &deepest2);
	colt_nested_cells in2(inner2, 3, head2, "Months");
	colt_nested_cells in3(inner3, 3, head2, "Months");
	colt_nested_cells in4(inner4, 3, head2, "Months");

	colt_nested_cells out11(outer1, 4, head1, "Dates", &in11);
	colt_nested_cells out11a(outer1, 4, head1, "Dates", &in12);
	colt_nested_cells out12(outer1, 4, head1, "Dates", &in2);
	colt_nested_cells out13(outer1, 4, head1, "Dates", &in3);

	colt_nested_cells out21(outer2, 4, head1, "Dates", &in4);
	colt_nested_cells out22(outer2, 4, head1, "Dates", &in3);
	colt_nested_cells out23(outer2, 4, head1, "Dates", &in11);

	colt_nested_cells out31(outer3, 4, head1, "Dates", &in2);
	colt_nested_cells out32(outer3, 4, head1, "Dates", &in11);
	colt_nested_cells out33(outer3, 4, head1, "Dates", &in4);

	std::ofstream mine("mine.yml");
//	out11.start();
	out11.nested_output(NULL, 0, &mine);
	out11a.nested_output(&out11, 0, &mine);
	out12.nested_output(&out11a, 0, &mine);

	out23.nested_output(&out12, 0, &mine);
	out21.nested_output(&out23, 0, &mine);
	out22.nested_output(&out21, 0, &mine);

	out31.nested_output(&out22, 0, &mine);
	out32.nested_output(&out31, 0, &mine);
	out33.nested_output(&out32, 0, &mine);

//	out13.end(xxx+1);
//	out11.end();

}

//colt_nested_cells::colt_nested_cells(colt_datatype **c, int cols, char **heads, char *k):
colt_nested_cells::colt_nested_cells(char **c, int cols, char **heads, char *k, colt_nested_cells *n, std::ostream *o):
	cells(c),
	num_cols(cols),
	headers(heads),
	key(k),
	output_type(colt_nested_cells::yml),
	pretty(1),
	next(n),
	out(o),
	index(-1)
{
	// TODO Auto-generated constructor stub
	if(!out)
		out = &cout;
}

colt_nested_cells::~colt_nested_cells() {
	// TODO Auto-generated destructor stub
}

int colt_nested_cells::nested_output(colt_nested_cells *old, int level, std::ostream *o)
{
	if(output_type == yml)
		return yml_output(old, level, o);
	else if(output_type == json)
		return json_output(old, level, o);
}

int colt_nested_cells::json_output(colt_nested_cells *old, int level, std::ostream *o)
{
	if(o)
		out = o;

	if(!old) {
		if(num_cols) {
//			gen_connection(level);
			*out << key << ":";
		}
//		gen_connection(level);
		*out << "[";
		return gen_json(level+1);
	}

	if(index != old->index)
		*out << "]";
	else
		if(next)
			return next->json_output(old->next, level+1, out);

	gen_connection(level);
//	start();

	return gen_json(level);
}

int colt_nested_cells::gen_json(int level)
{
	char tmp_str[COLT_MAX_STRING_SIZE];
	int retval = 1;

	if(!num_cols)
		if(next)
			return next->json_output(NULL, level+1, out);
		else
			return 1;

	if(num_cols && pretty)
		indent(level+1);

	*out << "{";

	if(pretty)
		indent(level+1);

	for(int j=0; j<num_cols; j++) {

		*out << "\"" << headers[j] << "\": \"" << cells[j] << "\"";

		if(j<num_cols-1)
			gen_connection(level+2);
	}

	if(next)
		retval = next->json_output(NULL, level+1, out);

	if(pretty)
		indent(level+1);

	*out << "}";

	return retval;
}

int colt_nested_cells::yml_output(colt_nested_cells *old, int level, std::ostream *o)
{
	if(o)
		out = o;

	if(!old) {
		if(key) {
			gen_connection(level);
			*out << key << ":";
			level++;
		}
		gen_connection(level);
		start();
		return gen_yml(level);
	}

	if(index != old->index) {
		if(key) {
			gen_connection(level-1);
			start();
			gen_connection(level);
			*out << key << ":";
		}
		level++;
	} else
		if(next)
			if(key)
				return next->yml_output(old->next, level+2, out);
			else
				return next->yml_output(old->next, level+1, out);

	gen_connection(level);
	start();
	return gen_yml(level);
}

void colt_nested_cells::gen_connection(int level)
{
	if(output_type == json)
		*out << ",";

	if(pretty)
		indent(level);
}

int colt_nested_cells::indent(int level)
{
	*out << "\n";
	for(int i=0; i<level; i++)
		*out << "  ";
}

int colt_nested_cells::gen_yml(int level)
{
	char tmp_str[COLT_MAX_STRING_SIZE];
	int retval = 1;

	if(num_cols && pretty)
		indent(level+1);

	for(int j=0; j<num_cols; j++) {

		*out << headers[j] << ": " << cells[j];

		if(j<num_cols-1)
			gen_connection(level+1);
	}

	if(next)
		retval = next->yml_output(NULL, level+1, out);

	return retval;
}

int colt_nested_cells::gen_close(int level)
{
//	if(pretty)
//		indent(level);
//
//	cout << "}";

	return 1;
}
