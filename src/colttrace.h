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

	const	char *string;
	int		is_match;
public:
	static 	int	show;

	colt_trace(const char *str);
	virtual ~colt_trace();

	void add_match(const char *s);
};

#endif /* COLTTRACE_H_ */
