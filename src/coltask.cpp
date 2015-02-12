/*
 * coltask.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltask.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

colt_ask::colt_ask(char *cexp, char *host, char *p):
	colt_base(),
	colt_expression(NULL),
	hostname(NULL),
	column_count(0)
{
	// TODO Auto-generated constructor stub
	colt_expression = new char[strlen(cexp)+1];
	strcpy(colt_expression, cexp);
	hostname = new char[strlen(host)+1];
	strcpy(hostname, host);
	port = new char[strlen(p)+1];
	strcpy(port, p);

	   int portno, n;
	    struct sockaddr_in serv_addr;
	    struct hostent *server;

	    portno = atoi(port);
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0)
	        perror("ERROR opening socket");
	    server = gethostbyname(hostname);
	    if (server == NULL) {
	        perror("Ask, gethostbyname");
	        exit(0);
	    }
	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    bcopy((char *)server->h_addr,
	         (char *)&serv_addr.sin_addr.s_addr,
	         server->h_length);
	    serv_addr.sin_port = htons(portno);
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
	        perror("Ask connect");
	        exit(-1);
	    }

	    n = write(sockfd,cexp,strlen(cexp));
	    if (n < 0) {
	         perror("ERROR writing expression to socket");
	         exit(-1);
	    }

	    int ok;

	    n = read(sockfd, &ok, sizeof(ok));
		if (n < 0) {
			 perror("Tell verifying command line.");
			 exit(-1);
		}
}

colt_ask::~colt_ask() {
	// TODO Auto-generated destructor stub
}

int colt_ask::num_cols()
{
	return column_count;
}

int colt_ask::get_next_row()
{
	cout << "qqq colt_ask::get_next_row()\n";
	for(int i=0; i<column_count; i++) {
		char field[COLT_MAX_STRING_SIZE];
		int n;
		n = read(sockfd, field, COLT_MAX_STRING_SIZE);
		if (n < 0) {
			perror("Ask read");
			return -1;
		}

		if(strcmp(field, "__EOD__") == 0)
			return -1;

		if(ask_fields[i])
			delete ask_fields[i];

		ask_fields[i] = new char[strlen(field)+1];
		strcpy(ask_fields[i], field);

		int ok;
		n = write(sockfd, &ok, sizeof(ok));
		if(n<0){
			perror("Ask read verify");
			return -1;
		}
	}

	return 1;
}

char **colt_ask::fields(int rec_num)
{
	cout << "qqq cotl_ask::fields()\n";
	return ask_fields;
}

colt_datatype **colt_ask::cells(int rec_num)
{
	cout << "cells\n";
	return NULL;
}

char **colt_ask::col_headers()
{
	cout << "qqq cotl_ask::col_headers()\n";
	return headers;
}

int colt_ask::preprocess()
{
	cout << "qqq cotl_ask::preprocess()\n";
    int n = read(sockfd,&column_count,sizeof(column_count));
    if (n < 0)
    	return 0;

    headers = (char **) malloc(sizeof(char *) * column_count);

    for(int i=0; i<column_count; i++) {
    	char head[COLT_MAX_STRING_SIZE];
        n = read(sockfd, head, COLT_MAX_STRING_SIZE);
        if (n < 0)
        	return 0;
    	headers[i] = new char[strlen(head)+1];
    	strcpy(headers[i], head);

    	int ok;
    	write(sockfd, &ok, sizeof(ok));
    }

    ask_fields = (char **) malloc(sizeof(char *) * column_count);
    for(int j=0; j<column_count; j++)
    	ask_fields[j] = NULL;

    return out_object->preprocess();
}
//
//int colt_ask::process(int rec_num)
//{
//	return out_object->process(rec_num);
//}

void colt_ask::postprocess()
{
    close(sockfd);
    colt_base::postprocess();
}
