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
	else if(version_number == colt_range_version)
		retval = new colt_range(file_name);
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
	preload(1),
	iterate_count(0),
	end_index(-1)
{
	i_am = colt_class_thru;
	strcpy(file_name, source);
//	load(source);
}

coltthru::~coltthru()
{
	// TODO Auto-generated destructor stub
	//free(index_list);
}

colt_base *coltthru::copy(colt_base *op)
{
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
	return colt_thru_version;
}

void coltthru::initialize()
{
	index_list = (int *) malloc(index_record_size * 100);
	index_count = 0;
}


int coltthru::num_lines()
{
	return index_count;
};

void coltthru::push_back(int i)
{
	index_list[index_count++] = i;
};

void coltthru::fill_sequential(int start)
{

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
	push_back(xx);
	return *this;
}

char *coltthru::source_file_name()
{
	if(file_name[0])
		return file_name;
	return colt_operator::source_file_name();
}

int coltthru::write_config(FILE *out)
{
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
    colt_thru_identifier ident;

    memcpy((void *) &ident, (void *) index_list, sizeof(ident));

    index_count = ident.index_count;

	if(match(ident.file_name, "thru$"))
		operand = colt_load_thru(ident.file_name);
	else
		operand = new colt_csv(ident.file_name, 1);
    operand->out_object = this;

    return base_ptr + sizeof(ident)/sizeof(int);
}

void coltthru::save(char *file_name)
{
	FILE *out = fopen(file_name, "w");
	write_config(out);
	fwrite(index_list, index_record_size, index_count, out);
	fclose(out);
}

int coltthru::load(char *file_name, int status)
{
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
	iterate_count = beg;
	if(end >=0)
		end_index = end;
	else
		end_index = index_count;
}

int coltthru::show_status(char *base_ptr, int indent)
{
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
	return operand->fields(index_list[rec_num]);
}

int coltthru::get_next_row()
{
	if(iterate_count > end_index)
		return -1;

//	return index(iterate_count++);
	return iterate_count++;
}

void coltthru::process_all()
{
	if(!preload)
		return operand->process_all();

	colt_base::process_all();
}

int coltthru::preprocess()
{
	if(preload)
		return out_object->preprocess();

	index_count = 0;

	if(index_list)
		free(index_list);

	index_list = (int *) malloc(max_size());

	if(out_object)
		return out_object->preprocess();

	return 1;
}

int coltthru::process(int rec_num)
{
	if(preload)
		return out_object->process(rec_num);

	push_back(rec_num);
	return 1;
}

void coltthru::postprocess()
{
	if(!preload) {
		if(file_name[0])
			save(file_name);
		preload = 1;
		for(int i=0; i<index_count; i++)
			process(i);
	}

	if(out_object)
		out_object->postprocess();
}
