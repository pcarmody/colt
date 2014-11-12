/*
 * coltreduce.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltreduce.h"

colt_reduce::colt_reduce(colt_base &in, int n):
	colt_operator(in),
	level(n),
	source(NULL),
	datasource(NULL)
{
	// TODO Auto-generated constructor stub

}
colt_reduce::colt_reduce(colt_base &in, char *dsource):
		colt_operator(in),
		level(99),
		source(NULL),
		datasource(NULL)
{
	// TODO Auto-generated constructor stub
	source = new char[strlen(dsource)+1];
	strcpy(source, dsource);
}

colt_reduce::~colt_reduce() {
	// TODO Auto-generated destructor stub
}

char **colt_reduce::fields(int rec_num)
{
	return datasource->fields(rec_num);
}

colt_datatype **colt_reduce::cells(int rec_num)
{
	return datasource->cells(rec_num);
}

int colt_reduce::preprocess()
{
	// verify/find the datasource in question.
	datasource = get_datasource(level);
	return colt_operator::preprocess();
}

int colt_reduce::process(int rec_num)
{
	int new_rec_num = reduce_to(level, rec_num);
	return colt_operator::process(new_rec_num);
}
