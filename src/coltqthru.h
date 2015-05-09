/*
 * coltqthru.h
 *
 *  Created on: May 4, 2015
 *      Author: paul
 */

#ifndef COLTQTHRU_H_
#define COLTQTHRU_H_

class colt_qthru : public colt_operator {
	colt_queuethru	*qthru;
	coltthru		*last_thru;
	char			*file_name;
	int				new_thru;

public:
	colt_qthru(colt_base &in, char *fname);
	virtual ~colt_qthru();

	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTQTHRU_H_ */
