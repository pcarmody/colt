/*
 * coltuniq.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltuniq.h"

colt_uniq::colt_uniq(colt_base &in, char *col, int l):
	colt_onchange(in, NULL, col),
	list(),
	last(l)
{
	// TODO Auto-generated constructor stub
	i_am = colt_class_uniq;
}

colt_uniq::~colt_uniq() {
	// TODO Auto-generated destructor stub
}


int colt_uniq::process(int rec_num)
{
	COLT_TRACE("colt_uniq::process(int rec_num)")

	char **row = fields(rec_num);
	char *key = row[column_number];

	if(!list.IsItem(key)) {
		list.AddItem(key, rec_num);
		if(!last)
			return colt_operator::process(rec_num);
	} else if(last)
		list.Reset(key, rec_num);

	return 1;
}

void colt_uniq::postprocess()
{
	COLT_TRACE("colt_uniq::postprocess()")
	if(last)
		for(int i=0; i<list.Size(); i++)
			colt_operator::process(list[i]);

	return colt_operator::postprocess();

}
