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
	char	*header;

	colt_datatype(int t=COLT_DATATYPE, char *h=NULL) { type = t; value_type = NULL; header = h; };
	~colt_datatype() {};

	void set_value(void *val, int init=0)
	{
		switch(type) {
		case COLT_DATATYPE:
			value_type = (value_type_t *) val;
			break;
		case COLT_DT_INTEGER:
//			cout << "qqq " << (long) val <<":"<< *(int *) val << "\n";
			tmp_value.long_val = *(int *) val;
			value_type = &tmp_value;
			break;
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			consume_thru(val);
		}
	}
	int size()
	{
		switch(type) {
		case COLT_DATATYPE:
			return strlen(value_type->str_val)+1;
			break;
		case COLT_DT_INTEGER:
			return 10;
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			return thru_size();
//			perror("size of thru hasn't been implemented.\n");
//			exit(1);
		}
	}
	int thru_size();
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
			perror("Attempted to assign a string to a source datatype.\n");
			exit(1);
		}
	}
	void operator =(long &l) {
		switch(type) {
		case COLT_DATATYPE:
			sprintf(tmp_value.str_val, "%ld", l);
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
			perror("Attempted to assign an integer to a source datatype.\n");
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
			return "coltthru *";
		case COLT_DT_SORT:
			return "coltsort *";
		case COLT_DT_CTHRU:
			return "colt_cthru *";
		case COLT_DT_RANGE:
			return "colt_range *";
		case COLT_DT_BITMAP:
			return "colt_bitmap *";
		default:
			return "void *";
		}
	}

	void set_buffer(char *x) {
		if(type >= COLT_DT_SOURCE)
			parse_thru(x);
		else if(type == COLT_DT_INTEGER) {
			tmp_value.long_val = atol(x);
			value_type = &tmp_value ;
		} else
			set_value(x);
	};
	char *parse_thru(char *x);
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
			return malloc(sizeof(void *));
		}
	};
	int format(char *x) {
		switch(type) {
		case COLT_DATATYPE:
			strcpy(x, value_type->str_val);
			break;
		case COLT_DT_INTEGER:
			tmp_value = *value_type;
			sprintf(x, "%ld", tmp_value.long_val);
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
			len = size();
//			len=strlen(value_type->str_val);
			break;
		case COLT_DT_INTEGER:
			len=sizeof(int);
			break;
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			return generate_thru(x);
		}
		fwrite(value_type, len, 1, (FILE *) x);
//		memcpy(x, value_type, len);
		return len;
	};
	int generate_thru(void *x);
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
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			return consume_thru(x);
		}
		value_type = (value_type_t *) x;
		x += len;
		return x;
	};
	char *consume_thru(void *x);
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
		case COLT_DT_SOURCE:
		case COLT_DT_THRU:
		case COLT_DT_SORT:
		case COLT_DT_CTHRU:
		case COLT_DT_RANGE:
		case COLT_DT_BITMAP:
			return compare_thru(rite);
		}
		return 0;
	}
	int compare_thru(colt_datatype &rite);
	int set_type(int t)
	{
		return type = t;
	}
};

colt_datatype *new_datatype(int t, char *h=NULL);

#endif /* COLTDATATYPE_H_ */
