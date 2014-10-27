/*
 * coltthru.h
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */

#ifndef COLTTHRU_H_
#define COLTTHRU_H_

typedef struct {
	int	id;
	char file_name[100];
	int index_count;
} colt_thru_identifier;

#define colt_thru_version 1
#define colt_sort_version 100
#define colt_cthru_version 200
#define colt_range_version 300
#define colt_bitmap_version 400
#define colt_keyspace_version 500

colt_base *colt_load_thru(char *file_name, int status=0);
int match(const char *string, char *pattern);

class coltthru : public colt_operator {
public:
	char		file_name[100];
	int			*index_list;
	int			index_count;
	int			iterate_count;
	int			index_record_size;
	int			preload;
	int			end_index;

	coltthru(colt_base &in, char *destination_file_name=NULL);
	coltthru(char *source=NULL);
	virtual ~coltthru();

	virtual colt_base *copy(colt_base *op=NULL);

	int preloaded() { return preload; };
	void initialize();
	void fill_sequential(int start=0);
	int num_lines();
	coltthru &operator <<(int xx);
	void save(char *file_name);
	int load(char *file_name, int status=0);
	char *source_file_name();
	virtual void push_back(int i);
	virtual	int serialize_id_num();
	virtual int write_config(FILE *);
	virtual int *read_config(int *);
//	virtual int index(int i);
	virtual void set_begin_end_index(int beg, int end=-1);
	virtual int show_status(char *baseptr, int indent=0);

	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);
	int get_next_row();
	void process_all();
	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTTHRU_H_ */
