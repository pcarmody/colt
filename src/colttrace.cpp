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
	location(str),
	is_match(0)
{
	// TODO Auto-generated constructor stub
	for(int i=0; i<matches.size() && !is_match; i++) {
		const char *c = matches[i].c_str();
		is_match = match((const char *) str, (char *) c);
	}

	if (show && is_match) {
		indent();
		std::cout << ">" << location << "\n";
		indent_level++;
	}
}

colt_trace::~colt_trace()
{
	// TODO Auto-generated destructor stub
	if(show && is_match) {
		indent_level--;
		indent();
		std::cout << "<" << location << "\n";
	}
}

void colt_trace::add_match(const char *str)
{
	matches.push_back(str);
}

colt_trace &colt_trace::start()
{
	if(show && is_match) {
		indent();
		std::cout << "=" << location << ":";
	}

	return *this;
}

colt_trace &colt_trace::operator <<(const char *str)
{
	if(show && is_match) {
		std::cout << str;
	}

	return *this;
}

colt_trace &colt_trace::operator <<(char *str)
{
	if(show && is_match) {
		std::cout << str;
	}

	return *this;
}

colt_trace &colt_trace::operator <<(int x)
{
	if(show && is_match) {
		std::cout << x;
	}

	return *this;
}

colt_trace &colt_trace::operator <<(colt_datatype *dtype)
{
	if(show && is_match) {
		char buff[COLT_MAX_STRING_SIZE];
		dtype->format(buff);
		std::cout << buff;
	}

	return *this;
}
