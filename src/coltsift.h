/*
 * coltsift.h
 *
 *  Created on: Nov 28, 2014
 *      Author: paul
 */

#ifndef COLTSIFT_H_
#define COLTSIFT_H_

class colt_sift : public colt_operator {
	colt_base	*source;
	char		*source_name;
	char		*index_name;
	char		*col_name;
	int			col_number;
public:
	colt_sift(colt_base &in, char *fname, char *iname, char *cname);
	virtual ~colt_sift();

	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTSIFT_H_ */
