/*
 * coltoutcbf.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltoutcbf.h"

colt_out_cbf::colt_out_cbf(colt_base &in, char *fname):
	colt_operator(in),
	cbf(fname),
	init(1)
{

}

colt_out_cbf::~colt_out_cbf()
{

}

int colt_out_cbf::preprocess()
{
	COLT_TRACE("int colt_out_cbf::preprocess()")

	return colt_operator::preprocess();
}

int colt_out_cbf::process(int i)
{
	COLT_TRACE("colt_out_cbf::process(int i)");

	colt_datatype **some_cells = cells(i);

	if(init) {
		char types[COLT_MAX_NUM_COLS];

		for(int i=0; i<num_cols(); i++)
			types[i] = some_cells[i]->type;

		cbf.start_build(100, num_cols(), col_headers(), types);
		init = 0;
	}

	cbf.write_cells(some_cells);
	return colt_operator::process(i);
}

void colt_out_cbf::postprocess()
{
	COLT_TRACE("colt_out_cbf::postprocess()")
	cbf.finish_build();
	colt_operator::postprocess();
}
