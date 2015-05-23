/*
 * coltyml.h
 *
 *  Created on: May 21, 2015
 *      Author: paul
 */

#ifndef COLTYML_H_
#define COLTYML_H_

class colt_yml : public colt_operator {
public:
	char			*file_name;
	std::ostream	*out;
	colt_nested_cells	*old;

	colt_yml(colt_base &in, char *fname);
	virtual ~colt_yml();

	int preprocess();
	int process(int rec_num);
};

#endif /* COLTYML_H_ */
