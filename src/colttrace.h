/*
 * colttrace.h
 *
 *  Created on: Nov 19, 2014
 *      Author: paulcarmody
 */

#ifndef COLTTRACE_H_
#define COLTTRACE_H_

class colt_trace {
	static	int indent_level;
	static 	vector<string>	matches;

	const	char *location;
	int		is_match;
public:
	static 	int	show;

	colt_trace(const char *str);
	virtual ~colt_trace();

	void add_match(const char *s);
	void indent() { for(int i=0; i<indent_level; i++) std::cout << " "; };
	colt_trace &start();
	colt_trace &operator <<(const char *str);
	colt_trace &operator <<(char *str);
	colt_trace &operator <<(int x);
	colt_trace &operator <<(colt_datatype *dtype);
};

#endif /* COLTTRACE_H_ */
