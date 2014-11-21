/*
 * coltdatatype.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#include "colt_headers.h"
#include "coltdatatype.h"


//
//  COLT_INTEGER
//

colt_datatype::colt_datatype() {
	// TODO Auto-generated constructor stub
	type = COLT_DATATYPE;
}

colt_datatype::~colt_datatype() {
	// TODO Auto-generated destructor stub
}

void colt_datatype::set_buffer(char *x)
{
	COLT_TRACE("colt_datatype::set_buffer(char *x)")
	buffer = x;
}

int colt_datatype::format(char *x)
{
	COLT_TRACE("colt_datatype::format(char *x)")
	strcpy(x, buffer);
	return strlen(buffer);
}

int colt_datatype::generate(char *x)
{
	COLT_TRACE("colt_datatype::generate(char *x)")
	return format(x);
}

char *colt_datatype::consume(char *x)
{
	COLT_TRACE("*colt_datatype::consume(char *x)")
	if(buffer)
		delete buffer;

	int len = strlen(x)+1;
	buffer = new char[len];
	strcpy(buffer, x);

	return x+len;
}

int colt_datatype::gen_header(char *x)
{
	COLT_TRACE("colt_datatype::gen_header(char *x)")
	memcpy(x, &type, sizeof(type));

	return sizeof(type);
}

int colt_datatype::operator <(colt_datatype &right)
{
	COLT_TRACE("colt_datatype::operator <(colt_datatype &right)")
	return strcmp(buffer, right.buffer) < 0;
}

int colt_datatype::operator <(char *x)
{
	COLT_TRACE("colt_datatype::operator <(char *x)")
	return strcmp(buffer, x) < 0;
}

//
//  COLT_INTEGER
//

colt_integer::colt_integer():
		colt_datatype()
{
	type = COLT_DT_INTEGER;
}

colt_integer::~colt_integer()
{

}

void colt_integer::set_buffer(char *x)
{
	COLT_TRACE("colt_integer::set_buffer(char *x)")
	colt_datatype::set_buffer(x);
	value = atoi(x);
}

int colt_integer::format(char *x)
{
	COLT_TRACE("colt_integer::format(char *x)")
	char tmp[100];
//	itoa(x, tmp, 10);
	sprintf(tmp, "%d", value);
	strcat(x, tmp);

	return strlen(tmp);
}

int colt_integer::generate(char *x)
{
	COLT_TRACE("colt_integer::generate(char *x)")
	memcpy(x, &value, sizeof(value));
	return sizeof(value);
}

char *colt_integer::consume(char *x)
{
	COLT_TRACE("*colt_integer::consume(char *x)")
	value = (int) *x;
	return x+sizeof(value);
}

int colt_integer::operator <(char *x)
{
	COLT_TRACE("colt_integer::operator <(char *x)")
	return value < atoi(x);
}

int colt_integer::operator <(colt_datatype &right)
{
	COLT_TRACE("colt_integer::operator <(colt_datatype &right)")
	if(right.type == COLT_DT_INTEGER) {
		colt_integer *tmp = (colt_integer *) &right;
		return value < tmp->value;
	}

	return value < atoi(right.buffer);
}

