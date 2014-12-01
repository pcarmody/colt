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
	COLT_DT_INDEX_LIST,
	COLT_DT_RANGE,
	COLT_DT_BITMAP
};

class colt_datatype {
public:
	char	*buffer;
	short	type;

	colt_datatype();
	virtual ~colt_datatype();

	static colt_datatype * new_object(int type);

	virtual void *get_value();
	virtual char *gen_value_type();

	virtual	void set_buffer(char *x);
	virtual	int format(char *x);
	virtual int generate(char *x);
	virtual	char *consume(char *x);
	virtual int gen_header(char *x);
	virtual int	isa_container() { return type >= COLT_DT_INDEX; };
	virtual int operator <(char *x);
	virtual int operator <(colt_datatype &x);
			int operator >=(colt_datatype &x) { return !operator <(x); };
			int operator >=(char *x) { return !operator <(x); };
};

class colt_integer : public colt_datatype {
public:
	int	value;

	colt_integer();
	virtual ~colt_integer();

	virtual void *get_value();
	virtual char *gen_value_type();

	virtual	void set_buffer(char *x);
	virtual	int format(char *x);
	virtual int generate(char *x);
	virtual	char *consume(char *x);
	virtual int operator <(char *x);
	virtual int operator <(colt_datatype &x);
};

#endif /* COLTDATATYPE_H_ */
