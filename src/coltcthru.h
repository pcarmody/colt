/*
 * coltcthru.h
 *
 *  Created on: May 19, 2014
 *      Author: paulcarmody
 */

#ifndef COLTCTHRU_H_
#define COLTCTHRU_H_

#pragma once
#include <stdlib.h>
#include <iostream>
#include "coltbase.h"
#include "coltoperator.h"
#include "coltthru.h"
#include "coltsort.h"

class colt_range;

using namespace std;
class colt_cthru : public colt_sort {
public:
	class colt_record {
	public:
		int		record_number;
		int		min_value_below;
		int		max_value_below;
		int		num_nodes_below;
//		int		sum_nodes_below;
		char	marked;				// set if this node is marked
		char	mark_all_below;		// set if all nodes below should be marked
		char	all_marked_below;	// set if all nodes below are marked
	};

	char *max_record(colt_record *node) { return node->max_value_below >= 0 ? record(node->max_value_below): NULL; };
	char *min_record(colt_record *node) { return node->min_value_below >= 0 ? record(node->min_value_below): NULL; };
	char *max_field_val(colt_record *node) { return node->max_value_below >= 0 ? field_val(node->max_value_below, col_num()): NULL; };
	char *min_field_val(colt_record *node) { return node->min_value_below >= 0 ? field_val(node->min_value_below, col_num()): NULL; };

	void search_lowest_low(int low, int high);
	void search_highest_high(int low, int high);
	void print(int indent=0, int low=0, int high=-1) {

		if(high == -1) {
			high = num_lines()-1;
		}

		int center = low + (high - low)/2;

		if(low <= center-1)
			print(indent+1, low, center-1);
		cout << center << ":" <<
				node_list[center].record_number << ":" <<
				node_list[center].num_nodes_below << ":";// <<
//				node_list[center].sum_nodes_below << "\t";
		for(int i=0; i<indent; i++)
			cout << "..";

		cout << extract_str(field_val(center, col_num()))
			 << " min:" << node_list[center].min_value_below << "=" << extract_str(min_field_val(&node_list[center]))
			 << " max:" << node_list[center].max_value_below << "=" << extract_str(max_field_val(&node_list[center]))
			 << "\n";

		if(center+1 <= high)
			print(indent+1, center+1, high);
	};

	//
	// main class declarations
	//

	colt_record	*node_list;

	colt_cthru(char *source);
	colt_cthru(colt_base &in, int col_num, int numeric=0, int asc=1);
	colt_cthru(colt_base &b, char *col_name, int numeric=0, int asc=1);
	colt_cthru(colt_base &b, char *file, char *col_name, int numeric=0, int asc=1);
	virtual ~colt_cthru();

	void initialize();
	colt_record *init(int low=0, int high=-1);
	virtual int search(char *cond, colt_range *retobj);
	virtual int search(char *min, char *max, colt_range *retobj);
	void save(char *file_name);

	char *record(int line_num);
	char *field_val(int rec_num, int col_num);
	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);

	virtual void push_back(int i);
	virtual int *read_config(int *);
	int serialize_id_num();
	int index(int i);

	void perform_sort();
	int preprocess();
	void postprocess();
};

#endif /* COLTCTHRU_H_ */
