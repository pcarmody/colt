/*
 * coltpthru.cpp
 *
 *  Created on: May 26, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltpthru.h"

colt_pthru::colt_pthru(colt_base &in, char *exp, char *col, char *f):
	colt_onchange(in, exp, col),
	list(),
	filename(NULL)
{
	i_am = colt_class_pthru;

	if(f) {
		filename = new char[strlen(f)+1];
		strcpy(filename, f);
	}
}

colt_pthru::~colt_pthru() {
	// TODO Auto-generated destructor stub

	if(filename)
		delete filename;
}

colt_operator *colt_pthru::insert_expression(char *expression)
{
	COLT_TRACE("*colt_pthru::insert_expression(char *expression)")
	colt_operator *retval;
	colt_parser parse(expression);

	retval = new colt_operator;
	retval->operand = operand;

	parse.parse_unary_expressions(retval);

//	retval->set_destination(out_object, 0);

	return retval;
}

int colt_pthru::process(int rec_num)
{
	COLT_TRACE("colt_pthru::process(int rec_num)")
	int retval = 0;
	char **row = fields(rec_num);
	char *key = row[column_number];
	colt_operator *expression = NULL;
	colt_operator *endl = NULL;

	if(!list.IsItem(key)) {

		expression = insert_expression(expression_string);
		endl = (colt_operator *) expression->get_destination();

		if(endl->is_a(colt_class_cthru)
		|| endl->is_a(colt_class_sort)
		|| endl->is_a(colt_class_thru)
		|| endl->is_a(colt_class_range)
		|| endl->is_a(colt_class_bitmap)
//		|| endl->is_a(colt_class_pthru)
//		|| endl->is_a(colt_class_pathru)
		|| endl->is_a(colt_class_queuethru)
//		|| endl->is_a(colt_class_jthru)
		)

		endl->set_destination(out_object, 1);

		if(!expression->preprocess())
			return 0;

		endl->out_object = NULL;
		list.AddItem(key, expression);
		end_of_list.AddItem(key, endl);

	} else {
		expression = list[key];
		endl = end_of_list[key];
	}

	endl->set_destination(out_object, 1);
	expression->process(rec_num);
	endl->out_object = NULL;

	return retval;
}

void colt_pthru::postprocess()
{
	COLT_TRACE("colt_pthru::postprocess()");

	FILE *append = fopen(filename, "w");

	vector<string> keys = end_of_list.get_keys();
	std::sort(keys.begin(), keys.end());

	for(int i=0; i<keys.size(); i++) {
//		end_of_list[i]->set_destination(out_object, 1);
		string key = keys[i];
		cout << "qqq " << key << "\n";
		list[key]->postprocess();

		coltthru *thru = (coltthru *) end_of_list[key];

		if(thru->is_a(colt_class_queuethru))
			fprintf(append, "%s\n", thru->source_file_name());
		else {
			char tmp[COLT_MAX_STRING_SIZE];
			coltthru *mythru = (coltthru*) thru;
			mythru->to_string(tmp);
			fprintf(append, "%s\t%s\n", tmp, key.c_str());
		}
		end_of_list[i]->out_object = NULL;
	}

	fclose(append);

	return colt_operator::postprocess();
}
