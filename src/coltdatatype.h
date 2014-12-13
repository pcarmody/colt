/*
 * coltdatatype.h
 *
 *  Created on: Oct 24, 2014
 *      Author: paulcarmody
 */

#ifndef COLTDATATYPE_H_
#define COLTDATATYPE_H_

#include <stdlib.h>

enum Colt_DataType_Enum {
	COLT_DATATYPE,
	COLT_DT_INTEGER,
	COLT_DT_SOURCE,
	COLT_DT_THRU,
	COLT_DT_SORT,
	COLT_DT_CTHRU,
	COLT_DT_RANGE,
	COLT_DT_BITMAP
};

class colt_datatype {
public:
	union value_type_t {
		char str_val[100];
		long long_val;
//		short short_val;
//		double double_val;
//		float float_val;
//		colt_dt_index_val index_val;
//		colt_dt_range_val range_val;
//		colt_dt_bitmap_val bitmap_val;
	} *value_type, tmp_value;
	short	type;

	colt_datatype(int t=COLT_DATATYPE) { type = t; value_type = NULL; };
	~colt_datatype() {};

	void set_value(void *val, int init=0) {
//		if(init)
//			value_type = (value_type_t *) make_space();
		value_type = (value_type_t *) val;
//		switch(type){
//		case COLT_DATATYPE:
//			strcpy(value_type->str_val, (char *) val);
//			break;
//		case COLT_DT_INTEGER:
//			value_type->long_val = *(long *) val;
//			break;
//		}
	}
	void operator =(char *str) {
		switch(type) {
		case COLT_DATATYPE:
			value_type = (value_type_t *) str;
			break;
		case COLT_DT_INTEGER:
			value_type = &tmp_value;
			tmp_value.long_val = atol(str);
			break;
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			perror("Attempted to assign a string to a soure datatype.\n");
			exit(1);
		}
	}
	void operator =(long &l) {
		switch(type) {
		case COLT_DATATYPE:
			sprintf(tmp_value.str_val, "%d", l);
			value_type = &tmp_value;
			break;
		case COLT_DT_INTEGER:
			value_type = (value_type_t *) &l;
			break;
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			perror("Attempted to assign an integer to a soure datatype.\n");
			exit(1);
		}
	};
	void operator =(colt_datatype &colt) {
		type = colt.type;
		value_type = (value_type_t *) colt.make_space();
		set_value(colt.value_type);
	}
	void *get_value() { return value_type; };
	char *gen_value_type() {
		switch(type){
		case COLT_DATATYPE:
			return "char *";
		case COLT_DT_INTEGER:
			return "long *";
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			return "void *";
		}
	}

	void set_buffer(char *x) { set_value(x); };
	void *make_space() {
		switch(type) {
		case COLT_DATATYPE:
			return malloc(100);
		case COLT_DT_INTEGER:
			return malloc(sizeof(long));
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			return malloc(sizeof(value_type_t));
		}
	};
	int format(char *x) {
		switch(type) {
		case COLT_DATATYPE:
			strcpy(x, value_type->str_val);
			break;
		case COLT_DT_INTEGER:
			sprintf(x, "%d", value_type->long_val);
			break;
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			return format_thru(x);
		}

		return strlen(x);
	};
	int format_thru(char *x);
	int generate(char *x) {
		int len=0;
		switch(type) {
		case COLT_DATATYPE:
			len=strlen(value_type->str_val);
			break;
		case COLT_DT_INTEGER:
			len=sizeof(int);
			break;
		}
		memcpy(x, value_type, len);
		return len;
	};
	char *consume(char *x, int t=-1) {
		if(t>0)
			type = t;
		int len=0;
		switch(type) {
		case COLT_DATATYPE:
			len=strlen(x);
			break;
		case COLT_DT_INTEGER:
			len=sizeof(int);
			break;
		}
		value_type = (value_type_t *) x;
		x += len;
		return x;
	};
	int gen_header(char *x){
		memcpy(x, &type, sizeof(type));

		return sizeof(type);
	};
	int	isa_container() { return type >= COLT_DT_SOURCE; };
	int compare(colt_datatype &rite) {
		switch(type) {
		case COLT_DATATYPE:
			return strcmp(value_type->str_val, rite.value_type->str_val);
		case COLT_DT_INTEGER:
			return  value_type->long_val - rite.value_type->long_val;
		}
		return 0;
	}
	int set_type(int t)
	{
		return type = t;
	}
};

colt_datatype *new_datatype(int t);

class colt_integer : public colt_datatype {
public:
//	long	value;

	colt_integer(): colt_datatype(COLT_DT_INTEGER) {};
	~colt_integer() {};

//	virtual void *get_value();
//	virtual char *gen_value_type();
//
//	virtual	void set_buffer(char *x);
//	virtual void set_value(void *);
//	virtual void *make_space();
//	virtual	int format(char *x);
//	virtual int generate(char *x);
//	virtual	char *consume(char *x);
//	virtual int operator <(char *x);
//	virtual int operator <(colt_datatype &x);
//	virtual int compare(colt_datatype &r);
};

#endif /* COLTDATATYPE_H_ */
