/*
 * coltoutcbf.h
 *
 *  Created on: Oct 27, 2014
 *      Author: paulcarmody
 */

#ifndef COLTOUTCBF_H_
#define COLTOUTCBF_H_

class colt_out_cbf : public colt_operator {
	char 	*file_name;
	FILE	*fileptr;
public:
	colt_out_cbf(colt_base &in, char *fname=NULL);
	virtual ~colt_out_cbf();

	virtual int preprocess();
	virtual int process(int i);
	virtual void postprocess();
};

#endif /* COLTOUTCBF_H_ */
