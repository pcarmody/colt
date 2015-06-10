/*
 * coltcbf.cpp
 *
 *  Created on: May 29, 2015
 *      Author: paul
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "colt_headers.h"
#include "coltcbf.h"

#define LOC(X,Y) X*num_cols() + Y

colt_cbf::colt_cbf(char *fname):
	colt_base(),
	headers(NULL),
	coltypes(NULL),
	lookup(NULL),
	data_blob(NULL),
	file_name(NULL),
	file_descr(NULL),
 	current_offset(NULL),
 	base_ptr(NULL),
	my_cells(NULL),
	my_fields(NULL)
{
	// TODO Auto-generated constructor stub

	file_name = new char[strlen(fname)+1];
	strcpy(file_name, fname);
}

colt_cbf::~colt_cbf() {
	// TODO Auto-generated destructor stub
	delete file_name;
}

int colt_cbf::num_cols()
{
	COLT_TRACE("int colt_cbf::num_cols()")
	return config.num_cols;
}

int colt_cbf::num_rows()
{
	COLT_TRACE("int colt_cbf::num_rows()")
	return config.num_rows;
}

char **colt_cbf::col_headers()
{
	COLT_TRACE("int colt_cbf::col_headers()")
	return headers;
}

int colt_cbf::start_build(int rows, int cols, char **h, char *types)
{
	COLT_TRACE("int colt_cbf::start_build(int rows, int cols, char **h, char *types)")
	file_descr = fopen(file_name, "wb");

	config.file_type = colt_cbf_version;
	config.num_cols = cols;
	config.num_rows = 0;
	config.data_offset = 0;

	current_offset = 0;

	int len = sizeof(config);
	fwrite(&config, len, 1, file_descr);
	current_offset += len;

	len = sizeof(char) * cols;
	fwrite(types, len, 1, file_descr);
	current_offset += len;

	for(int i=0; i<cols; i++) {
		len = strlen(h[i])+1;
		fwrite(h[i], len, 1, file_descr);
		current_offset += len;
	}

	config.data_offset = current_offset;

	lookup = (int *) malloc(sizeof(int) * rows * cols);

	return current_offset;
}

int colt_cbf::write_cells(colt_datatype **d)
{
	COLT_TRACE("int colt_cbf::write_cells(colt_datatype **d)")
	for(int i=0; i<config.num_cols; i++) {
		lookup[LOC(config.num_rows, i)] = current_offset;
		int len = 0;
//		if(d[i]->type >= COLT_DT_THRU)
			len = d[i]->generate((char *) file_descr);
//		else {
//			len = d[i]->size();
//			fwrite(d[i]->value_type, len, 1, file_descr);
//		}
		current_offset += len;
	}

	config.num_rows++;

	return current_offset;
}

int colt_cbf::finish_build()
{
	COLT_TRACE("int colt_cbf::finish_build()")
	config.lookup_offset = current_offset;

	for(int i=0; i<config.num_rows; i++)
		for(int j=0; j<config.num_cols; j++) {
//			cout << "qqq " << i << ":" << j << ":"<< lookup[LOC(i,j)] << "\n";
			fwrite(&lookup[LOC(i,j)], sizeof(*lookup), 1, file_descr);
		}
//	int len = config.num_rows * config.num_cols * sizeof(int);
//	fwrite(lookup, len, 1, file_descr);

	rewind(file_descr);

	fwrite(&config, sizeof(config), 1, file_descr);

	fclose(file_descr);

	return 1;
}

int colt_cbf::open_and_load()
{
	COLT_TRACE("int colt_cbf::open_and_load()")
	struct stat sb;
	int fd = open (file_name, O_RDONLY);
	if (fd == -1) {
		perror ("Unable to open CBF file.");
		return NULL;
	}

	if (fstat (fd, &sb) == -1) {
			perror ("fstat error on CBF file.");
			return NULL;
	}

	if (!S_ISREG (sb.st_mode)) {
			fprintf (stderr, "%s is not a file\n", file_name);
			return NULL;
	}

	base_ptr = (int *) mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

	close(fd);

	data_blob = (void *) base_ptr + config.data_offset;

	memcpy(&config, base_ptr, sizeof(config));

	coltypes = (char *) base_ptr + sizeof(config);

	headers = (char **) malloc(sizeof(char *) * config.num_cols);
	int i=0;
	int j =0;
	char *heads = (char *) base_ptr + sizeof(config) + sizeof(char) * config.num_cols;
	headers[i++] = heads;
	while(i < config.num_cols) {
		if(!*heads++)
			headers[i++] = heads;
	}

	lookup = ((int *)  base_ptr) + config.lookup_offset/sizeof(int);
	data_blob = ((char*) base_ptr) + config.data_offset;

//	my_cells = new colt_datatype[config.num_cols];
	my_cells = (colt_datatype **) malloc (sizeof(colt_datatype *) * config.num_cols);

	for(int i=0; i<config.num_cols; i++) {
		my_cells[i] = new colt_datatype;
		my_cells[i]->type = coltypes[i];
	}

	current_row = 0;
}

int colt_cbf::get_next_row()
{
	COLT_TRACE("int colt_cbf::get_next_row()")
	if(current_row == config.num_rows)
		return -1;
	return current_row++;
}

char **colt_cbf::fields(int rec_num)
{
	COLT_TRACE("int colt_cbf::fields(int rec_num)")
	if(!my_fields) {
		my_fields = (char **) malloc( sizeof(char *) * config.num_cols);
		for(int i=0; i<config.num_cols; i++)
			my_fields[i] = NULL;
	}

	cells(rec_num);

	for(int i=0; i<config.num_cols; i++) {
		if(my_cells[i]->type != COLT_DATATYPE) {
			if(my_fields[i])
				delete my_fields[i];
			my_fields[i] = new char[my_cells[i]->size()];
			my_cells[i]->format(my_fields[i]);
		} else
			my_fields[i] = (char *) my_cells[i]->value_type;
	}

	return my_fields;
}

void *colt_cbf::data_ptr(int n, int m)
{
	COLT_TRACE("int colt_cbf::data_ptr(int n, int m)");
	int base = *lookup;
	int offset = lookup[LOC(n,m)];
	return (char *) data_blob + offset - base;
//	return (char *) data_blob + lookup[LOC(n,m)] - *lookup;
}

colt_datatype **colt_cbf::cells(int rec_num)
{
	COLT_TRACE("int colt_cbf::cells(int rec_num)")
	for(int i=0; i<config.num_cols; i++) {
		my_cells[i]->set_value( data_ptr(rec_num, i) );
	}

	return my_cells;
}
