/*
 * coltqueuethru.h
 *
 *  Created on: May 1, 2015
 *      Author: paul
 */

#ifndef COLTQUEUETHRU_H_
#define COLTQUEUETHRU_H_

class colt_queuethru : public colt_csv {
	class colt_qthru_thru_struct {
	public:
		int sum;
		colt_base *thru;
		colt_qthru_thru_struct():
			sum(0),
			thru(NULL)
		{};
		~colt_qthru_thru_struct() {};
	};
	vector<colt_qthru_thru_struct *>		thru_list;

public:
	colt_queuethru(char *file_name, char col_sep='\t', char eol_sep='\n', char q_char='\0');
	virtual ~colt_queuethru();

	virtual int append_thru(colt_base *thru);
	virtual int open_and_load();

	virtual int num_lines();
	virtual int num_cols();
	virtual char **col_headers();
	virtual char **fields(int rec_num);

	virtual int process(int rec_num);
};

#endif /* COLTQUEUETHRU_H_ */
