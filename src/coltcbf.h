/*
 * coltcbf.h
 *
 *  Created on: May 29, 2015
 *      Author: paul
 */

#ifndef COLTCBF_H_
#define COLTCBF_H_

class colt_cbf : public colt_base {
public:
	struct config_t {
		int 	file_type;
		int		num_cols;
		int		num_rows;
		int		data_offset;
		int		lookup_offset;
	} config;
	char	**headers;
	char	*coltypes;
	int		*lookup;
	void	*data_blob;
	char	*file_name;
	FILE	*file_descr;
	int 	current_offset;
	void 	*base_ptr;
	colt_datatype **my_cells;
	char	**my_fields;
	int		current_row;
    char	**metadata_fields;
    char	*meta_file_type;
    colt_datatype	**metadata_cells;

	colt_cbf(char *fname);
	virtual ~colt_cbf();

	int open_and_load();

	int num_cols();
	int num_lines();
	char **col_headers();
	char **fields(int rec_num);
	colt_datatype **cells(int rec_num);
	int get_next_row();
	char *source_file_name();

	virtual int	meta_num_cols();
	virtual char *meta_col_header(int n);
	virtual char **meta_col_headers();
	virtual char **meta_fields(int rec_num);
	virtual colt_datatype **meta_cells(int rec_num);
	virtual	int	get_meta_row(int rec_num);

	int start_build(int rows, int cols, char **h, char *types);
	int write_cells(colt_datatype **d);
	int finish_build();
	void *data_ptr(int n, int m);
};

#endif /* COLTCBF_H_ */
