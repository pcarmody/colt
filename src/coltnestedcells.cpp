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

//	out11.start();
	out11.nested_output(NULL);
	out11a.nested_output(&out11);
	out12.nested_output(&out11);
//	out13.nested_output(&out12);
	out21.nested_output(&out12);
//	out22.nested_output(&out21);

//	out13.end(xxx+1);
//	out11.end();

}

//colt_nested_cells::colt_nested_cells(colt_datatype **c, int cols, char **heads, char *k):
colt_nested_cells::colt_nested_cells(char **c, int cols, char **heads, char *k, colt_nested_cells *n):
	cells(c),
	num_cols(cols),
	headers(heads),
	key(k),
	output_type(colt_nested_cells::json),
	pretty(1),
	next(n)
{
	// TODO Auto-generated constructor stub

}

colt_nested_cells::~colt_nested_cells() {
	// TODO Auto-generated destructor stub
}

int colt_nested_cells::nested_output(colt_nested_cells *old, int level)
{
	if(!old) {
		gen_connection(level);
		std::cout << key << ":";
		gen_connection(level);
		start();
		return gen_row(level);
	}

	if(strcmp(key, old->key) == 0)
		if(next)
			return next->nested_output(old->next, level+1);

	gen_connection(level);
	start();

	return gen_row(level);
}

void colt_nested_cells::gen_connection(int level)
{
	if(pretty)
		indent(level);
}

int colt_nested_cells::indent(int level)
{
	std::cout << "\n";
	for(int i=0; i<level; i++)
		std::cout << "  ";
}

int colt_nested_cells::gen_row(int level)
{
	char tmp_str[COLT_MAX_STRING_SIZE];

	if(pretty)
		indent(level+1);

	for(int j=0; j<num_cols; j++) {

		std::cout << headers[j] << ": " << cells[j];

		if(j<num_cols-1)
			gen_connection(level+1);
	}

	if(next) {
		return next->nested_output(NULL, level+1);
	}

	return 1;
}

int colt_nested_cells::gen_close(int level)
{
//	if(pretty)
//		indent(level);
//
//	cout << "}";

	return 1;
}
