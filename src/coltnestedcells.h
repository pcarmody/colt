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
	std::ostream	*out;
	int				index;

	enum {
		xml,
		yml,
		html,
		json,
		ruby
	};

//	colt_nested_cells(colt_datatype **c, int cols, char **headers, char *key);
	colt_nested_cells(char **c, int cols, char **headers, char *key=NULL, colt_nested_cells *n=NULL, std::ostream *o=NULL);
	virtual ~colt_nested_cells();

	void start() { *out << "-";};
	void end(int level) { for(int i=0; i<level; i++) std::cout << "\n]"; };

	int indent(int level);
	int nested_output(colt_nested_cells *old, int level=0, std::ostream *o=NULL);
	int yml_output(colt_nested_cells *old, int level=0, std::ostream *o=NULL);
	int json_output(colt_nested_cells *old, int level=0, std::ostream *o=NULL);
	void gen_connection(int level);
	int gen_yml(int level);
	int gen_json(int level);
	int gen_close(int level);
};

#endif /* COLTNESTEDCELLS_H_ */
