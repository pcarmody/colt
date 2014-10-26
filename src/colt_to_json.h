/*
 * colt_to_json.h
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */

#ifndef COLT_TO_JSON_H_
#define COLT_TO_JSON_H_

class colt_to_json : public colt_out {
	char	*key_col;
	int		key_col_index;
public:
	colt_to_json(colt_base &in, char* key);
	virtual ~colt_to_json();

	virtual colt_base *copy(colt_base *op=NULL);

	int preprocess();
	int process(int rec);
	void postprocess();
};
#endif
