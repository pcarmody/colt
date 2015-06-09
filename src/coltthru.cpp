/*
 * coltthru.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: paulcarmody
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "colt_headers.h"

using namespace std;
#include <regex.h>

/*
 * Match string against the extended regular expression in
 * pattern, treating errors as no match.
 *
 * return 1 for match, 0 for no match
 */

int match(const char *string, char *pattern)
{
    int    status;
    regex_t    re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* report error */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);      /* report error */
    }
    return(1);
}

colt_base *colt_load_thru(char *file_name, int status)
{
	COLT_TRACE("*colt_load_thru(char *file_name, int status)")

	char *extension = strrchr(file_name, '.');

	if(strcmp(extension, ".cbf") == 0) {
		colt_cbf *retval = new colt_cbf(file_name);
		retval->open_and_load();
		return retval;
	}

	if(strcmp(extension, ".qthru") == 0) {
		colt_queuethru *retval = new colt_queuethru(file_name);
		retval->open_and_load();
		return retval;
	}

	if(strcmp(extension, ".pthru") == 0) {
		colt_partthru *retval = new colt_partthru(file_name);
		retval->open_and_load();
		return retval;
	}

//	if(strcmp(extension, ".zsv") == 0) {
	if(strcmp(extension, ".thru") != 0) {
		colt_zsv *retval = new colt_zsv(file_name, 1);
		retval->open_and_load();
		return retval;
	}

//	if(strcmp(extension, ".csv") == 0) {
//		colt_csv *retval = new colt_csv(file_name, 1);
//		retval->open_and_load();
//		return retval;
//	}
//
//	if(strcmp(extension, ".thru") != 0) {
//		char error_msg[100];
//		sprintf(error_msg, "Unable to determine file type for '%s'.\n", file_name);
//		perror(error_msg);
//		return NULL;
//	}

	struct stat sb;
	int fd;

	fd = open (file_name, O_RDONLY);
	if (fd == -1) {
		perror ("open");
		return NULL;
	}

	if (fstat (fd, &sb) == -1) {
			perror ("fstat");
			return NULL;
	}

	if (!S_ISREG (sb.st_mode)) {
			fprintf (stderr, "%s is not a file\n", file_name);
			return NULL;
	}

	int *index_list = (int *) mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

	int version_number = *index_list;
	close(fd);

//	index_count = sb.st_size/index_record_size - 1;

	// load the file name

	coltthru *retval = NULL;
	if(version_number == colt_thru_version)
		retval = new coltthru(file_name);
	else if(version_number == colt_sort_version)
		retval = new colt_sort(file_name);
	else if(version_number == colt_cthru_version)
		retval = new colt_cthru(file_name);
//	else if(version_number == colt_range_version)
//		retval = new colt_range(file_name);
	else if(version_number == colt_bitmap_version)
		retval = new coltbitmap(file_name);
	else if(version_number == colt_keyspace_version)
		retval = new colt_keyspace(file_name);

	if(retval) {
		retval->load(file_name, status);
	}

	return retval;
}

//
//  coltthru
//

coltthru::coltthru(colt_base &in, char *dest_file_name):
		colt_operator(in),
		index_list(NULL),
		index_count(0),
		index_record_size(sizeof(int)),
		preload(0),
		iterate_count(0),
		end_index(-1)
{
	i_am = colt_class_thru;
	if(dest_file_name)
		strcpy(file_name, dest_file_name);
	else
		file_name[0] = '\0';
}


coltthru::coltthru(char *source):
	colt_operator(),
	index_list(NULL),
	index_count(0),
	index_record_size(sizeof(int)),
	preload(0),
	iterate_count(0),
	end_index(-1)
{
	i_am = colt_class_thru;
	if(source)
		strcpy(file_name, source);
	else
		file_name[0] = '\0';
//	load(source);
}

coltthru::~coltthru()
{
	// TODO Auto-generated destructor stub
	//free(index_list);
}

colt_base *coltthru::copy(colt_base *op)
{
	COLT_TRACE("*coltthru::copy(colt_base *op)")
	coltthru *retval = new coltthru(*op);
	int size = sizeof(int) * index_count;
	retval->index_list = (int *) malloc(size);
	memcpy((void *) retval->index_list, (void *) index_list, size);
	retval->index_count = index_count;
	retval->index_record_size = index_record_size;
	return retval;
}

int coltthru::serialize_id_num()
{
	COLT_TRACE("coltthru::serialize_id_num()")
	return colt_thru_version;
}

void coltthru::initialize()
{
	COLT_TRACE("coltthru::initialize()")
	index_list = (int *) malloc(index_record_size * 100);
	index_count = 0;
}


int coltthru::num_lines()
{
	COLT_TRACE("coltthru::num_lines()")
	return index_count;
};

void coltthru::push_back(int i)
{
	COLT_TRACE("coltthru::push_back(int i)")
	if(index_count >= buffer_size) {
		buffer_size += max_size();
		index_list = (int *) realloc(index_list, buffer_size);
	}
	index_list[index_count++] = i;
};

void coltthru::fill_sequential(int start)
{
	COLT_TRACE("coltthru::fill_sequential(int start)")

	if(index_list)
		free(index_list);

	index_list = (int *) malloc(index_record_size * operand->num_lines());

//	for(colt_counter *i=get_row_counter(); !i->done(); i->next())
//		index_list[index_count++] = i->index();
//	for(int i=start; i<operand->num_lines(); i++)
//		index_list[index_count++] = i;
}

coltthru &coltthru::operator <<(int xx)
{
	COLT_TRACE("&coltthru::operator <<(int xx)")
	push_back(xx);
	return *this;
}

int coltthru::contains(int rec_num)
{
	for(int i=0; i<index_count; i++)
		if(index_list[i] == rec_num)
			return 1;
	return 0;
}

char *coltthru::source_file_name()
{
	COLT_TRACE("*coltthru::source_file_name()")
	if(file_name[0])
		return file_name;
	return colt_operator::source_file_name();
}

char *coltthru::index_file_name()
{
	if(!operand)
		return NULL;
	return operand->source_file_name();
}

colt_base *coltthru::get_datasource(int count)
{
	COLT_TRACE("*coltthru::get_datasource(int count)")
	if(count == 0)
		return this;
	return operand->get_datasource(count-1);
}

int coltthru::reduce_to(int level, int rec_num)
{
	COLT_TRACE("coltthru::reduce_to(int level, int rec_num)")
	int retval = index_list[rec_num];
	if(level == 0)
		return retval;
	return operand->reduce_to(level-1, retval);
}

int coltthru::write_config(FILE *out)
{
	COLT_TRACE("coltthru::write_config(FILE *out)")
	colt_thru_identifier ident;
	ident.id = serialize_id_num();
	ident.index_count = index_count;
	strcpy(ident.file_name, colt_operator::source_file_name());
	ident.id = colt_thru_version;

	fwrite(&ident, 1, sizeof(ident), out);
	return sizeof(ident);
}

int *coltthru::read_config(int *base_ptr)
{
	COLT_TRACE("*coltthru::read_config(int *base_ptr)")
    colt_thru_identifier ident;

    memcpy((void *) &ident, (void *) index_list, sizeof(ident));

    index_count = ident.index_count;

	operand = colt_load_thru(ident.file_name);
    operand->out_object = this;

    return base_ptr + sizeof(ident)/sizeof(int);
}


int	coltthru::meta_num_cols()
{
	return operand->meta_num_cols();
}

char *coltthru::meta_col_header(int n)
{
	return operand->meta_col_header(n);
}

char **coltthru::meta_col_headers()
{
	return operand->meta_col_headers();
}

char **coltthru::meta_fields(int rec_num)
{
	char **retval = operand->meta_fields(rec_num);

	retval[1] = new char[10];
	sprintf(retval[1], "%d", num_lines());

	retval[3] = new char[10];
	sprintf(retval[1], "%d", iterate_count);

	retval[7] = "thru";

	return retval;
}

colt_datatype **coltthru::meta_cells(int rec_num)
{
	colt_datatype **retval = operand->meta_cells(rec_num);

	return retval;
}

int	coltthru::get_meta_row(int rec_num)
{
	return operand->get_meta_row(rec_num);
}

int coltthru::to_string(char *x)
{
	COLT_TRACE("coltthru::to_string(char *x)")
	sprintf(x, "thru:%s", colt_operator::source_file_name());
	char tmp[20];

	for(int i=0; i<index_count; i++) {
		sprintf(tmp, ",%d", index_list[i]);
		strcat(x, tmp);
	}

	return strlen(x);
}

int coltthru::format_size()
{
	return strlen(colt_operator::source_file_name())+1 + 6*index_count;
}

char *coltthru::from_string(char *input)
{
	COLT_TRACE("coltthru::from_string(char *input)")
	char file_name[COLT_MAX_STRING_SIZE];

	char *b = input+5;
	char *c = file_name;
	while(*b && *b != ',')
		*c++ = *b++;
	*c = '\0';

//	sscanf(input, "thru:%s,", file_name);

	while(*input && *input != ',') input++;

	if(!input)
		return input;

	int count = 0;
	char *a = input+1;
	while(*a)
		if(*a++ == ',')
			count++;

	index_list = (int *) malloc(count+1 * sizeof(int));
	index_count = 0;
	operand = colt_load_thru(file_name);
    operand->out_object = this;

	while(*input++ == ',') {
		int index = atoi(input);
		push_back(index);
		while(*input && *input != ',') input++;
	}

    end_index = index_count-1;

	return input;
}

int coltthru::generate(void *x)
{
	COLT_TRACE("coltthru::generate(void *x)")
	int len = 0;
	FILE *file_descr = (FILE *) x;

	int thru_type = colt_thru_version;
	fwrite(&thru_type, sizeof(thru_type), 1, file_descr);
	len += sizeof(thru_type);

	char *fname = colt_operator::source_file_name();
	fwrite(fname, strlen(fname)+1, 1, file_descr);
	len += strlen(fname)+1;

	fwrite(&index_count, sizeof(index_count), 1, file_descr);
	len += sizeof(index_count);

//	fwrite(index_list, sizeof(int), index_count, file_descr);
	for(int i=0; i<index_count; i++) {
		fwrite(&index_list[i], sizeof(index_list[i]), 1, file_descr);
		len += sizeof(index_list[i]);
	}

	return len;
}

int coltthru::consume(void *x)
{
	COLT_TRACE("coltthru::consume(void *x)");
	int len = sizeof(int);
	char *fname = (char *) x + len;
	operand = colt_load_thru(fname);
	operand->out_object = this;

	len += strlen(fname)+1;
	char *tmp = (char *)x + len;
	index_count = *(int *) tmp;
//	index_count = *(int *) x + len/sizeof(int);

	len += sizeof(index_count);
	tmp = (char *)x + len;
	index_list = (int *) tmp;

	len += sizeof(int) * index_count;

	return len;
}

void coltthru::save(char *file_name)
{
	COLT_TRACE("coltthru::save(char *file_name)")
	FILE *out = fopen(file_name, "w");
	write_config(out);
	fwrite(index_list, index_record_size, index_count, out);
	fclose(out);
}

int coltthru::load(char *file_name, int status)
{
	COLT_TRACE("coltthru::load(char *file_name, int status)")
    struct stat sb;
    char *p;
    int fd;

    fd = open (file_name, O_RDONLY);
    if (fd == -1) {
            perror ("open");
            return 1;
    }

    if (fstat (fd, &sb) == -1) {
            perror ("fstat");
            return 1;
    }

    if (!S_ISREG (sb.st_mode)) {
            fprintf (stderr, "%s is not a file\n", file_name);
            return 1;
    }

    p = (char *) mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (p == MAP_FAILED) {
            perror ("mmap");
            return 1;
    }

    index_list = (int *) p;

    // load the file name

//    if(*index_list != colt_thru_version)
//    	perror("Mismatched file data version.\n");
    index_list = read_config(index_list);
    end_index = index_count;
    preload = 1;

    if(status)
    	return show_status(p, status);

    return 1;
}

//int coltthru::index(int i)
//{
//	return index_list[i];
//}

void coltthru::set_begin_end_index(int beg, int end)
{
	COLT_TRACE("coltthru::set_begin_end_index(int beg, int end)")
	iterate_count = beg;
	if(end >=0)
		end_index = end;
	else
		end_index = index_count;
}

int coltthru::show_status(char *base_ptr, int indent)
{
	COLT_TRACE("coltthru::show_status(char *base_ptr, int indent)")
	 colt_thru_identifier ident;

	 memcpy((void *) &ident, (void *) base_ptr, sizeof(ident));

	 char indent_str[60];
	 for(int i=0; i<60; i++) indent_str[i] = ' ';
	 indent_str[indent*2] = '\0';

	 std::cout << indent_str << "sort_thru: " << file_name << "\n";
	 std::cout << indent_str << "num_lines: " << ident.index_count << "\n";
	 if(match(ident.file_name, "\\.thru")) {
		 std::cout << indent_str << "data_source:\n";
		 colt_load_thru(ident.file_name, indent+1);
	 } else
		 std::cout << indent_str << "data_source:" << ident.file_name << "\n";

	 return 1;
}

char **coltthru::fields(int rec_num)
{
	COLT_TRACE("**coltthru::fields(int rec_num)")
	return operand->fields(index_list[rec_num]);
}

colt_datatype **coltthru::cells(int rec_num)
{
	COLT_TRACE("**coltthru::cells(int rec_num)")
	return operand->cells(index_list[rec_num]);
}

colt_nested_cells *coltthru::nested_cells(int rec_num)
{
	COLT_TRACE("**coltthru::fields(int rec_num)")
	return operand->nested_cells(index_list[rec_num]);
}

int coltthru::get_next_row()
{
	COLT_TRACE("coltthru::get_next_row()")
	if(iterate_count > end_index)
		return -1;

//	return index(iterate_count++);
	return iterate_count++;
}

void coltthru::process_all()
{
	COLT_TRACE("coltthru::process_all()")
	if(!preload)
		return operand->process_all();

	colt_base::process_all();
}

int coltthru::preprocess()
{
	COLT_TRACE("coltthru::preprocess()")
	if(index_list)
		return colt_operator::preprocess();

	index_count = 0;

	buffer_size = 1000; //max_size();

	index_list = (int *) malloc(buffer_size * sizeof(int));

	return colt_operator::preprocess();
}

int coltthru::process(int rec_num)
{
	COLT_TRACE("coltthru::process(int rec_num)")
	_trace.start() << rec_num << "\n";
	if(preload)
		return colt_operator::process(rec_num);

	push_back(rec_num);
	return 1;
}

void coltthru::postprocess()
{
	COLT_TRACE("coltthru::postprocess()")
	if(!out_object)
		return;

	if(!preload) {
		if(end_index == -1) {
			iterate_count=0;
			end_index=index_count-1;
		}

		if(file_name[0])
			save(file_name);
		preload = 1;
		preprocess();
		for(int i=iterate_count; i<end_index; i++)
			process(i);
	}

	if(out_object)
		out_object->postprocess();
}
