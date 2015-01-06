/*
 * coltcexpression.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: paulcarmody
 */

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include "colt_headers.h"
#include "coltcexpression.h"
#define regex_match "\
int match(const char *string, char *pattern)\n\
{\n\
	int    status;\n\
	regex_t    re;\n\
	\n\
	if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {\n\
		return(0);      /* report error */\n\
	}\n\
	status = regexec(&re, string, (size_t) 0, NULL, 0);\n\
	regfree(&re);\n\
	if (status != 0) {\n\
		return(0);      /* report error */\n\
	}\n\
	return(1);\n\
}\n"

int colt_cexpression::expression_count = 0;
AssocArray<COLT_C_FUNC> cexpression_list;

colt_cexpression::colt_cexpression(colt_base &b, char *in, char *str):
	colt_operator(b),
	function_ptr(NULL),
	lookup_key(NULL)
{
	i_am = colt_class_c_expression;
	return_type = "int";
	code_string = new char[strlen(in)+1];
	strcpy(code_string, in);
	if(str) {
		lookup_key = new char[strlen(str)+1];
		strcpy(lookup_key, str);
		if(cexpression_list.IsItem(str))
			function_ptr = cexpression_list[str];
	}
}

colt_cexpression::colt_cexpression(colt_base &in, COLT_C_FUNC func):
	colt_operator(in),
	function_ptr(func)
{
	i_am = colt_class_c_expression;
	return_type = "int";
}

void colt_cexpression::compile_and_link()
{
	COLT_TRACE("colt_cexpression::compile_and_link()")
	if(function_ptr)
		return;

	char code_name[COLT_MAX_STRING_SIZE];
	char so_name[COLT_MAX_STRING_SIZE];
	char fn_name[COLT_MAX_STRING_SIZE];

	sprintf(code_name, "/tmp/ifcode_%d_%d.cc", (int) getpid(), expression_count);
	sprintf(so_name, "/tmp/ifcode_%d_%d.so", (int) getpid(), expression_count);
	sprintf(fn_name, "if_%d_%d", (int) getpid(), expression_count++);

	_trace.start() << ": code file name is " << code_name << "\n";
	std::ofstream c_code(code_name);
	c_code << "#include <iostream>\n";
	c_code << "#include <string.h>\n";
	c_code << "using namespace std;\n";

	c_code << "#include <sys/types.h>\n";
	c_code << "#include <regex.h>\n";

	c_code << regex_match;

	int cols = num_cols();
	colt_datatype **the_cells = cells(0);
//	for(int i=0; i<cols; i++)
//		c_code << "#define " << col_header(i) << " ((char *) row[" << i << "])\n";

	c_code << "extern \"C\" " << return_type << " " << fn_name << "(char **row)\n";
	c_code << "{\n";

	// sample output
	// int &cola = (int &) row[0];
	char **head = col_headers();
	for(int i=0; i<cols; i++) {
		_trace.start() << "i = " << i << ";" << head[i] << "\n";
		char *value_type = the_cells[i]->gen_value_type();
		if(!match(code_string, head[i]))
			continue;
		c_code << value_type
			<< " "
			<< head[i]
			<< " = ("
			<< value_type
			<< " ) row["
			<< i
			<< "];\n";
		if(!_trace)
//			cout << "name = " << name << "\n";
			c_code << "cout << \"" << head[i] << " = \" << " << head[i] << " << \";\\n\";\n";
	}

	c_code << "\treturn (long) " << code_string << ";\n";
	c_code << "}\n";
	c_code.close();
	_trace.start() << ":" << code_name << "\n";

	char pipe_string[COLT_MAX_STRING_SIZE];
	//	sprintf(pipe_string, "g++ -fPIC -shared %s -o %s", code_name, so_name);
	sprintf(pipe_string, "gcc -Wall -Werror -Werror=unused-variable -fpic -shared %s -o %s", code_name, so_name);

	FILE *pipe = popen(pipe_string, "r");

	if(pclose(pipe)) {
		perror("IF error: unable to compile the if function_ptrs.\n");
		exit(1);
	}

	void* handle = dlopen(so_name, RTLD_LAZY);

	if(!handle) {
		perror("IF option did not load properly.\n");
		exit(1);
	}

	function_ptr = (COLT_C_FUNC) dlsym(handle, fn_name);

	if(lookup_key)
		cexpression_list.AddItem(lookup_key, function_ptr);

}


colt_cexpression::~colt_cexpression() {
	// TODO Auto-generated destructor stub
}

int colt_cexpression::preprocess()
{
	COLT_TRACE("colt_cexpression::preprocess()")

	int retval =  colt_operator::preprocess();
	compile_and_link();

	return retval;
}

