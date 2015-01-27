/*
 * coltrace.cpp
 *
 *  Created on: Jan 26, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltrace.h"

colt_race::	colt_race(colt_base &in, char *col_name, char *left_name, char *right_name, char *flags, char *tstr):
	colt_operator(in),
	left_cells(NULL),
	right_cells(NULL),
	race_headers(NULL),
	xrefs(),
	race_cells(NULL),
	left_sort_col(-1),
	right_sort_col(-1)
{
	label = new char[strlen(col_name+1)];
	strcpy(label, col_name);
	left_sort_col_name = new char[strlen(left_name+1)];
	strcpy(left_sort_col_name, left_name);
	right_sort_col_name = new char[strlen(right_name+1)];
	strcpy(right_sort_col_name, right_name);
	command_string = new char[strlen(tstr+1)];
	strcpy(command_string, tstr);


	onequal_read_left = (flags[0] == '1');
	onequal_read_right = (flags[1] == '1');
}

colt_race::~colt_race() {
	// TODO Auto-generated destructor stub
	delete left_sort_col_name;
	delete right_sort_col_name;
	delete command_string;

	if(right_expression)
		delete right_expression;

	free( race_headers );
	free( race_cells );
}

colt_datatype **colt_race::cells(int rec_num)
{
	colt_race_xref *xref = xrefs[rec_num];

	if(xref->left != left_rec_num)
		left_cells = colt_operator::cells(xref->left);

	if(xref->right != right_rec_num)
		right_cells = right_expression->cells(xref->right);

	if(!race_cells)
		race_cells = (colt_datatype **) malloc( sizeof(colt_datatype *) * (left_num_cols + right_num_cols) );

	int i;
	for(i=0; i<left_num_cols; i++)
		race_cells[i] = left_cells[i];
	for(int j=0; j<right_num_cols; j++)
		race_cells[i++] = right_cells[j];

	return race_cells;
}

char **colt_race::fields(int rec_num)
{
	colt_race_xref *xref = xrefs[rec_num];
	char **left_fields;
	char **right_fields;

	left_fields = colt_operator::fields(xref->left);

	right_fields = right_expression->fields(xref->right);

	if(!race_fields)
		race_fields = (char **) malloc( sizeof(char *) * (left_num_cols + right_num_cols) );

	int i;
	for(i=0; i<left_num_cols; i++)
		race_fields[i] = left_fields[i];
	for(int j=0; j<right_num_cols; j++)
		race_fields[i++] = right_fields[j];

	return race_fields;
}

int colt_race::num_cols()
{
	return left_num_cols + right_num_cols;
}

char *colt_race::col_header(int n)
{
	COLT_TRACE("*colt_race::col_header(int n)")

	if(race_headers)
		return race_headers[n];

	if(n < left_num_cols)
		return colt_operator::col_header(n);

	return right_expression->col_header(n-left_num_cols);
}

char **colt_race::col_headers()
{
	if(race_headers)
		return race_headers;

	char **left_headers = colt_operator::col_headers();

	if(right_num_cols == 0)
		return left_headers;

	char **right_headers = right_expression->col_headers();

	race_headers = (char **) malloc( sizeof(char *)*(left_num_cols+right_num_cols) );

	int i=0;
	for(i=0; i<left_num_cols; i++) {
		race_headers[i] = left_headers[i];
		if(strcmp(left_headers[i], left_sort_col_name) == 0)
			left_sort_col = i;
	}

	for(int j=0; j<right_num_cols; j++) {
		char *tmp = new char[COLT_MAX_STRING_SIZE];
		sprintf(tmp, "%s.%s", right_expression->source_file_name(),right_headers[j]);
		race_headers[i+j] = tmp; //right_headers[j];
		if(strcmp(right_headers[j], right_sort_col_name) == 0)
			right_sort_col = j;
	}

	if(right_sort_col * left_sort_col < 0) {
		perror("Unable to match key column in race expression.\n");
		exit(-1);
	}

	return race_headers;
}

int colt_race::preprocess()
{
	colt_parser parse(command_string);

	right_expression = parse.parse(1);

	right_rec_num = 0;//right_expression->get_next_row();

	left_num_cols = colt_operator::num_cols();
	right_num_cols = right_expression->num_cols();

	col_headers();

	return colt_operator::preprocess();
}

int colt_race::index_and_process()
{
	left_cells = colt_operator::cells(left_rec_num);
	right_cells = right_expression->cells(right_rec_num);

	int out_rec_num = xrefs.size();
	xrefs.push_back(new colt_race_xref(left_rec_num, right_rec_num));

	return colt_operator::process(out_rec_num);
}

int colt_race::process(int rec_num)
{
	int comparator;
	left_rec_num = rec_num;

	int retval = index_and_process();

	comparator = left_cells[left_sort_col]->compare(*right_cells[right_sort_col]);

	if(onequal_read_left) {

		while(comparator > 0 && retval) {
			right_rec_num = right_expression->get_next_row();

			retval = index_and_process();

			comparator = left_cells[left_sort_col]->compare(*right_cells[right_sort_col]);
		}

		if(comparator == 0 && onequal_read_right)
			right_rec_num = right_expression->get_next_row();

		return retval;
	}

	// assume onequal_read_right is true.  if onequal_read_left == false and onequal_read_right == false is an error condition

	while(comparator >= 0 && retval) {
		right_rec_num = right_expression->get_next_row();

		retval = index_and_process();

		comparator = left_cells[left_sort_col]->compare(*right_cells[right_sort_col]);
	}

	return retval;
}
