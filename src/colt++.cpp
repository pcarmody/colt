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
#include "colttell.h"
#include "coltask.h"

#include <thread>

#include <stdio.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>
#include <regex>

#define BACKLOG     10  /* Passed to listen() */

void fatal_error(char const *err)
{
	cerr<< err;
	exit(-1);
}
/* Signal handler to reap zombie processes */
static void wait_for_child(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void handle(int newsock)
{
	COLT_TRACE("main handler.")

	char command[COLT_MAX_STRING_SIZE];
//	colt_tell tell(newsock);
    char buff[COLT_MAX_STRING_SIZE];
    int len = read(newsock, buff, COLT_MAX_STRING_SIZE);

    if(len < 0) {
    	perror("Listen handle command.");
    	exit(-1);
    }

	sprintf(command, "%s tell:%d", buff, newsock);

    cout << getpid() << ": Executing '" << command << "'\n";

    colt_parser parse(command);
    colt_base *expression = parse.parse();

    int ok = (expression) ? -1: 0;
    len = write(newsock, &ok, sizeof(ok));

    if(len < 0) {
    	perror("Listen verify command.");
    	exit(-1);
    }

//	colt_trace::show = 1;
//	_trace.add_match(".*");
	expression->process_all();

	cout << getpid() << ": Successfully processed " << expression->num_lines() << " rows\n";

	delete expression;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int listen_server(char *port)
{
    int sock;
    struct sigaction sa;
    struct addrinfo hints, *res;
    int reuseaddr = -1; /* True */

    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &res) != 0) {
        perror("Server Error: getaddrinfo");
        return 1;
    }

    /* Create the socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    /* Enable the socket to reuse the address */
    printf("setsockopt ...\n");

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("Server Error: setsockopt");
        return 1;
    }

    /* Bind to the address */
    printf("binding ...\n");

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(port));

    if( bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) > 0) {
//    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Server Error: bind");
        return 1;
    }

    /* Listen */
    printf("listening on %s...\n", port);

    if (listen(sock, BACKLOG) == -1) {
        perror("Server Error: listen");
        return 1;
    }

    freeaddrinfo(res);

    /* Set up the signal handler */
    sa.sa_handler = wait_for_child;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("Server Error: sigaction");
        return 1;
    }

    /* Main loop */
    while (1) {
        struct sockaddr_in their_addr;
        socklen_t size = sizeof(struct sockaddr_in);
        printf("listening ...");
        int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
        int pid;

        if (newsock == -1) {
            perror("Server Error: accept");
            return 0;
        }

        printf("Got a connection from %s on port %d\n", inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));

        pid = fork();
        if (pid == 0) {
            /* In child process */
            close(sock);
            handle(newsock);
            return 0;
        }
        else {
            /* Parent process */
            if (pid == -1) {
                perror("Server Error: fork");
                return 1;
            }
            else {
                close(newsock);
            }
        }
    }

    close(sock);

    return 0;
}

char *regex_sub(char *in, char *pattern, char *out)
{
	const std::string s = in; //"/home/toto/FILE_mysymbol_EVENT.DAT";
	std::smatch match;

	char outexp[COLT_MAX_STRING_SIZE];
	char expression[COLT_MAX_STRING_SIZE];
	char *a = expression;

	pattern++;
	while(*pattern && *pattern != '/') *a++ = *pattern++;
	*a = '\0';

	a = outexp;
	pattern++;
	while(*pattern && *pattern != '/') *a++ = *pattern++;
	*a = '\0';

	std::regex rgx(expression);

	std::regex_match(s, match, rgx);
    if (std::regex_search(s.begin(), s.end(), match, rgx)) {

		char *b = out;
		a = outexp;
		while(*a) {
			if(*a == '\\') {
				a++;
				if(isdigit(*a)) {
					int i = *a++ - '0';
					string c = match[i];
					strcpy(b, c.c_str());
					b += strlen(b);
				} else
					*b++ = *a;
			} else
				*b++ = *a++;
		}
    }

    return out;
}
int test_colt_nested_cells();
int main(int argc, char **argv)
{
	COLT_TRACE("main");

//	test_colt_nested_cells();
//	exit(1);
	colt_base *base_obj = NULL;
//	char out[COLT_MAX_STRING_SIZE];
//	std::cout << regex_sub("/home/toto/FILE_mysymbol_EVENT.DAT", "/.*FILE_(\\w+)_(\\w+)\\.DAT.*/alpha \\1 beta \\2 gamma/", out);
//	    exit(0);

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

	if(argc > 1 && strcmp(argv[1], "listen") == 0)
		return listen_server(argv[2]);

	if(argc > 1 &&
		(strncmp(argv[1], "-D", 2) == 0 || strcmp(argv[1], "--debug") == 0) ) {
		colt_trace::show = 1;

		while(strncmp(argv[i], "-D", 2) == 0)
			_trace.add_match(argv[i++]+2);
	}

	for(; i<argc; i++) {
		strcat(command_line, argv[i]);
		strcat(command_line, " ");
	}

	colt_parser parser(command_line);
	base_obj = parser.parse();

    base_obj->process_all();

    delete base_obj;

    return 0;
}
