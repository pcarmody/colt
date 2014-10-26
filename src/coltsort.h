/*
 * coltsort.h
 *
 *  Created on: May 7, 2014
 *      Author: paulcarmody
 */

#ifndef COLTSORT_H_
#define COLTSORT_H_

typedef struct {
	int	id;
	char file_name[100];
	int index_count;
	int column_number;
	int numeric_sort;
	int ascending;
} colt_sort_identifier;

class colt_range;

class colt_sort : public coltthru {
protected:
	int column_number;
	int numeric_sort;
	int ascending;
public:
	colt_sort(char *source);
	colt_sort(colt_base &b, int col_num=0, int numeric=0, int asc=1);
	colt_sort(colt_base &b, char *col_name, int numeric=0, int asc=1);
	colt_sort(colt_base &b, char *file, char *col_name, int numeric=0, int asc=1);
	virtual ~colt_sort();

	virtual colt_base *copy(colt_base *op=NULL);

	int get_column_number() { return column_number; };
	void initialize();
	int serialize_id_num();
	int write_config(FILE *);
	int *read_config(int *);
	int show_status(char *baseptr, int indent=0);
	int col_num() { return column_number; };
	int search(char *min, char *max, colt_range *retobj);
	void search_lowest_low(int low, int high);
	void search_highest_high(int low, int high);

	void postprocess();
	virtual void perform_sort();
};

#endif /* COLTSORT_H_ */
