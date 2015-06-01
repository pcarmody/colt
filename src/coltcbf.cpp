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
	my_cells(NULL)
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
	return config.num_cols;
}

int colt_cbf::num_rows()
{
	return config.num_rows;
}

char **colt_cbf::get_headers()
{
	return headers;
}

int colt_cbf::start_build(int rows, int cols, char **h, char *types)
{
	file_descr = fopen(file_name, "w");

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
	for(int i=0; i<config.num_cols; i++) {
		lookup[config.num_rows * i] = current_offset;
		int len = d[i]->size();
		fwrite(d[i]->value_type, len, 1, file_descr);
		current_offset += len;
	}

	config.num_rows++;

	return current_offset;
}

int colt_cbf::finish_build()
{
	config.lookup_offset = current_offset;

	int len = config.num_rows * config.num_cols * sizeof(int);
	fwrite(lookup, len, 1, file_descr);

	rewind(file_descr);

	fwrite(&config, sizeof(config), 1, file_descr);

	fclose(file_descr);

	return 1;
}

int colt_cbf::open_and_load()
{
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

	cout << "qqq file_type" << config.file_type << "\n";
	cout << "qqq num_cols" << config.num_cols << "\n";
	cout << "qqq num_rows" << config.num_rows << "\n";
	cout << "qqq data_offset" << config.data_offset << "\n";
	cout << "qqq lookup_offset" << config.lookup_offset << "\n";

	coltypes = (int *) ((char *) base_ptr + sizeof(config));

	headers = (char **) malloc(sizeof(char *) * config.num_cols);
	int i=0;
	int j =0;
	char *heads = (char *) base_ptr + sizeof(config) + sizeof(coltypes);
	headers[i++] = heads;
	while(i < config.num_cols) {
		if(!*heads++)
			headers[i++] = heads;
	}

	for(int k=0; k<config.num_cols; k++)
		cout << "qqq header " << k << ":" << headers[k] << "\n";

	lookup = (int *) ((char*) base_ptr) + config.lookup_offset;
	data_blob = ((char*) base_ptr) + config.data_offset;
	for(int n=0; n<config.num_rows; n++) {
		for(int m=0; m<config.num_cols; m++) {
			cout << n << ":" << m << " " << (char *) data_blob + lookup[n*m]  << "\t";
		}
		cout << "\n";
	}

	my_cells = new colt_datatype[config.num_cols];

	for(int i=0; i<config.num_cols; i++)
		my_cells[i].type = coltypes[i];

	exit(1);
}

char **colt_cbf::fields(int rec_num)
{

}

colt_datatype **colt_cbf::cells(int rec_num)
{
	for(int i=0; i<config.num_cols; i++)
		my_cells[i].set_value(data_blob + lookup[rec_num * i]);

	return &my_cells;
}
