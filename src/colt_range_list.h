/*
 * colt_range_list.h
 *
 *  Created on: June 25, 2014
 *      Author: paulcarmody
 */

#ifndef COLT_range_list_H_
#define COLT_range_list_H_

#pragma once

class colt_range_list : public colt_operator {
	colt_range	**ranges;
	int			range_count;
public:
	colt_range_list(colt_base &in);
	~colt_range_list();

	colt_base *copy(colt_base *op);

	int add_range(int low=0, int high=INT_MAX);
	int process(int rec_num);
};

#endif
