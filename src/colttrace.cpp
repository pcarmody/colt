/*
 * colttrace.cpp
 *
 *  Created on: Nov 19, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "colttrace.h"

int match(const char *string, char *pattern);

int colt_trace::indent_level=0;
int colt_trace::show=0;
vector<string> colt_trace::matches;

colt_trace::colt_trace(const char *str):
	string(str),
	is_match(0)
{
	// TODO Auto-generated constructor stub
	for(int i=0; i<matches.size() && !is_match; i++) {
		const char *c = matches[i].c_str();
		is_match = match((const char *) str, (char *) c);
	}

	if (show && is_match) {
		for(int i=0; i<indent_level; i++)
			std::cout << " ";
		std::cout << ">" << string << "\n";
		indent_level++;
	}
}

colt_trace::~colt_trace()
{
	// TODO Auto-generated destructor stub
	if(show && is_match) {
		indent_level--;
		for(int i=0; i<indent_level; i++)
			std::cout << " ";
		std::cout << "<" << string << "\n";
	}
}

void colt_trace::add_match(const char *str)
{
	matches.push_back(str);
}

colt_trace &colt_trace::operator <<(const char *str)
{
	if(show && is_match) {
		indent_level--;
		for(int i=0; i<indent_level; i++)
			std::cout << " ";
		std::cout << "<" << string << ":" << str;
	}

	return *this;
}
