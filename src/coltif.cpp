/*
 * coltif.cpp
 *
 *  Created on: May 8, 2014
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
#include "coltif.h"

colt_if::colt_if(colt_base &b, char *in):
	colt_cexpression(b, in)
{
	i_am = colt_class_if;
}

colt_if::colt_if(colt_base &b, COLT_C_FUNC cond):
	colt_cexpression(b, cond)
{
	i_am = colt_class_if;
}

colt_if::~colt_if() {
	// TODO Auto-generated destructor stub
}

int colt_if::process(int i)
{
	if((*function_ptr)(fields(i))) {
		return colt_cexpression::process(i);
	}
	return 0;
}
//colt_if::colt_if(colt_base &b, COLT_IF_FUNC cond):
//	colt_operator(b),
//	condition(cond)
//{
//	i_am = colt_class_if;
//}

//colt_if::colt_if(colt_base &b, char *in):
//	colt_operator(b),
//	condition(NULL)
//{
//	i_am = colt_class_if;
//	char code_name[80];
//	char so_name[80];
//	char fn_name[80];
//
//	sprintf(code_name, "/tmp/ifcode_%d.cc", (int) getpid());
//	sprintf(so_name, "/tmp/ifcode_%d.so", (int) getpid());
//	sprintf(fn_name, "if_%d", (int) getpid());
//
//	std::ofstream if_code(code_name);
//	if_code << "#include <iostream>\n";
////	if_code << "#include \"/tmp/colt_row.h\"\n";
//	if_code << "#include <sys/types.h>\n";
//	if_code << "#include <regex.h>\n";
//#define regex_match "\
//int match(const char *string, char *pattern)\n\
//{\n\
//    int    status;\n\
//    regex_t    re;\n\
//    \n\
//    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {\n\
//        return(0);      /* report error */\n\
//    }\n\
//    status = regexec(&re, string, (size_t) 0, NULL, 0);\n\
//    regfree(&re);\n\
//    if (status != 0) {\n\
//        return(0);      /* report error */\n\
//    }\n\
//    return(1);\n\
//}\n"
//	if_code << regex_match;
//	for(int i=0; i<num_cols(); i++)
//		if_code << "#define " << col_header(i) << " " << i << "\n";
//
//	if_code << "extern \"C\" int " << fn_name << "(char **row)\n";
//	if_code << "{\n";
////	if_code << "\t std::cout << \"if\" << row[name] << '\\n';";
//	if_code << "\treturn " << in << ";\n";
//	if_code << "}\n";
//	if_code.close();
//
//	char pipe_string[100];
////	sprintf(pipe_string, "g++ -fPIC -shared %s -o %s", code_name, so_name);
//	sprintf(pipe_string, "gcc -Wall -Werror -fpic -shared %s -o %s", code_name, so_name);
//
//	FILE *pipe = popen(pipe_string, "r");
//
//	if(pclose(pipe)) {
//		perror("IF error: unable to compile the if conditions.\n");
//		exit(1);
//	}
//
//	void* handle = dlopen(so_name, RTLD_LAZY);
//
//	if(!handle) {
//		perror("IF option did not load properly.\n");
//		exit(1);
//	}
//
//	condition = (COLT_IF_FUNC) dlsym(handle, fn_name);
//}
//
//colt_if::colt_if(colt_base &b, COLT_IF_FUNC cond):
//	colt_operator(b),
//	condition(cond)
//{
//	i_am = colt_class_if;
//}
//
//colt_if::~colt_if() {
//	// TODO Auto-generated destructor stub
//}
//
////char **colt_if::fields(int i)
//int colt_if::process(int i)
//{
//	if((*condition)(fields(i))) {
//		return colt_operator::process(i);
//	}
//	return NULL;
//}
