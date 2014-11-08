/*
 * coltcexpression.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: paulcarmody
 */

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


colt_cexpression::colt_cexpression(colt_base &b, char *in):
	colt_operator(b),
	condition(NULL)
{
	i_am = colt_class_c_expression;
	return_type = "int";
}

void colt_cexpression::compile_and_link()
{
	char code_name[80];
	char so_name[80];
	char fn_name[80];

	sprintf(code_name, "/tmp/ifcode_%d.cc", (int) getpid());
	sprintf(so_name, "/tmp/ifcode_%d.so", (int) getpid());
	sprintf(fn_name, "if_%d", (int) getpid());

	std::ofstream c_code(code_name);
	c_code << "#include <iostream>\n";
	//	c_code << "#include \"/tmp/colt_row.h\"\n";
	c_code << "#include <sys/types.h>\n";
	c_code << "#include <regex.h>\n";
	c_code << regex_match;
	for(int i=0; i<num_cols(); i++)
		c_code << "#define " << col_header(i) << " ((char *) row[" << i << "])\n";

	c_code << "extern \"C\" " << return_type << " " << fn_name << "(char **row)\n";
	c_code << "{\n";
	//	c_code << "\t std::cout << \"if\" << row[name] << '\\n';";
	c_code << "\treturn " << in << ";\n";
	c_code << "}\n";
	c_code.close();

	char pipe_string[100];
	//	sprintf(pipe_string, "g++ -fPIC -shared %s -o %s", code_name, so_name);
	sprintf(pipe_string, "gcc -Wall -Werror -fpic -shared %s -o %s", code_name, so_name);

	FILE *pipe = popen(pipe_string, "r");

	if(pclose(pipe)) {
		perror("IF error: unable to compile the if conditions.\n");
		exit(1);
	}

	void* handle = dlopen(so_name, RTLD_LAZY);

	if(!handle) {
		perror("IF option did not load properly.\n");
		exit(1);
	}

	condition = (COLT_C_FUNC) dlsym(handle, fn_name);
}


colt_cexpression::~colt_cexpression() {
	// TODO Auto-generated destructor stub
}

int colt_cexpression::preprocess()
{
	compile_and_link();

	return colt_operand::preprocess();
}

