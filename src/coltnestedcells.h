/*
 * coltnestedcells.h
 *
 *  Created on: May 20, 2015
 *      Author: paul
 */

#ifndef COLTNESTEDCELLS_H_
#define COLTNESTEDCELLS_H_

class colt_nested_cells {
public:
//	colt_datatype	**cells;
	char			**cells;
	int				num_cols;
	char			**headers;
	char			*key;
	colt_nested_cells	*next;
	int				output_type;
	int				pretty;

	enum {
		xml,
		yml,
		html,
		json,
		ruby
	};

//	colt_nested_cells(colt_datatype **c, int cols, char **headers, char *key);
	colt_nested_cells(char **c, int cols, char **headers, char *key=NULL, colt_nested_cells *n=NULL);
	virtual ~colt_nested_cells();

	void start() { std::cout << "-";};
	void end(int level) { for(int i=0; i<level; i++) std::cout << "\n]"; };

	int indent(int level);
	int nested_output(colt_nested_cells *old, int level=0);
	void gen_connection(int level);
	int gen_row(int level);
	int gen_close(int level);
};

#endif /* COLTNESTEDCELLS_H_ */
