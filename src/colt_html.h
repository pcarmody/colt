/*
 * colhtml.h
 *
 *  Created on: May 30, 2014
 *      Author: paulcarmody
 */

#ifndef COLTHTML_H_
#define COLTHTML_H_

class colt_html : public colt_out {
public:
	colt_html(colt_base &in);
	virtual ~colt_html();

	int headers_out();

	int preprocess();
	void postprocess();
	int process(int rec_num);
};
#endif
