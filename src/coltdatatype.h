/*
 * coltdatatype.h
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#ifndef COLTDATATYPE_H_
#define COLTDATATYPE_H_

enum Colt_DataType_Enum {
	COLT_DATATYPE,
	COLT_DT_INTEGER,
	COLT_DT_DATE,
	COLT_DT_INDEX,
	COLT_DT_RANGE,
	COLT_DT_BITMAP
};

class colt_datatype {
public:
	char	*buffer;

	colt_datatype();
	virtual ~colt_datatype();

	virtual	void set_buffer(char *x);
	virtual	int format(char *x);
	virtual int generate(char *x);
	virtual	char *consume(char *x);
};

class colt_integer : public colt_datatype {
public:
	int	value;

	colt_integer();
	virtual ~colt_integer();

	virtual	void set_buffer(char *x);
	virtual	int format(char *x);
	virtual int generate(char *x);
	virtual	char *consume(char *x);
};

#endif /* COLTDATATYPE_H_ */
