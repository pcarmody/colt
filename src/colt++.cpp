//============================================================================
// Name        : colt++.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <ctime>
//#include <string.h>
//#include <limits.h>
//#include "coltbase.h"
//#include "coltparser.h"
//#include "coltcsv.h"
//#include "coltoperator.h"
//#include "coltthru.h"
//#include "coltsort.h"
//#include "coltcthru.h"
//#include "coltif.h"
//#include "avl.h"
//#include "coltcthru.h"
//#include "AssocArray.h"
//
//#include "coltout.h"
//#include "colt_html.h"
//#include "colt_out_vertical.h"
//#include "colt_skip_limit.h"
//#include "colt_select.h"
//#include "colt_to_json.h"
//
//#include "colt_cross.h"
//#include "colt_range.h"
//#include "colt_range_list.h"
//
//using namespace std;
#include "colt_headers.h"
#include <thread>

void fatal_error(char const *err)
{
	cerr<< err;
	exit(-1);
}

int main(int argc, char **argv)
{
	COLT_TRACE("main");
	colt_base *base_obj = NULL;

	if(argc > 1 &&
		(strcmp(argv[1], "-h") == 0
		|| strcmp(argv[1], "help") == 0) ) {
//
//		if(argv[2]) {
//			colt_load_thru(argv[2], 1);
//			exit(0);
//		}

		cout << "colt++ filename <expressions> <output format>\n";
		cout << "\tcolumn-operations\n";
		cout << "\t\tselect:list\tselects given column names, i.e., select:id,name,state\n";
		cout << "\trow-operations\n";
		cout << "\t\tlimit:lim:skip\tlimits output to lim lines after first skipping skip lines.\n";
		cout << "\toutput format, choose one of: \n";
		cout << "\t\thtml\t\toutput should be in HTML format.\n";
		cout << "\t\t2json<:key>\toutput will be in JSON format, a hash based on column name key, otherwise and array.\n";
		cout << "\t\t2csv\t\toutput is in CSV format.\n";

		exit(0);
	}

	char command_line[1000];
	command_line[0] ='\0';
	int i = 1;

	if(argc > 1 &&
		(strcmp(argv[1], "-D") == 0 || strcmp(argv[1], "--debug") == 0) ) {
		colt_trace::show = 1;

		i = 2;
	}

	for(; i<argc; i++) {
		strcat(command_line, argv[i]);
		strcat(command_line, " ");
	}

	_trace.add_match("pars");

	colt_parser parser(command_line);
	base_obj = parser.parse();

    base_obj->process_all();

    delete base_obj;

    return 0;
}
