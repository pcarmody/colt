/*
 * colt_range.h
 *
 *  Created on: June 25, 2014
 *      Author: paulcarmody
 */

#ifndef COLT_RANGE_H_
#define COLT_RANGE_H_

#pragma once

typedef struct {
	int	id;
	char file_name[100];
	int min_value;
	int max_value;
} colt_range_identifier;

class colt_range : public coltthru {
public:
	long	min_value;
	long 	max_value;
	char 	*search_low;
	char	*search_high;

	colt_range(colt_base &in, char *low, char *high);
	colt_range(colt_base &in, int low, int high);
	colt_range(colt_base &in, char *destination_file_name=NULL);
	colt_range(colt_base &b, char *file, char *low, char *high);
	colt_range(char *fname);
	~colt_range();

	colt_base *copy(colt_base *op);

	colt_sort *parent_thru() { return (colt_cthru *) operand; };
	void set_low(long l) { min_value = l; };
	void set_high(long h) { max_value = h; };
	int in_range(int rec_num);
	int num_lines();

	virtual	int serialize_id_num();
	virtual int write_config(FILE *);
	virtual int *read_config(int *);
	int show_status(char *baseptr, int indent=0);

	void process_all();
	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif
