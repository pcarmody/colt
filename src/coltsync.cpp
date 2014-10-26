/*
 * coltsync.cpp
 *
 *  Created on: Sep 7, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include "coltsync.h"

colt_sync::colt_sync(colt_base &in):
	colt_operator(in)
{
	i_am = colt_class_sync;
	pthread_mutex_init(&push_mutex, NULL);
}

colt_sync::~colt_sync() {
	// TODO Auto-generated destructor stub
}

int colt_sync::process(int rec_num)
{
    pthread_mutex_lock(&push_mutex);

    int retval = colt_operator::process(rec_num);

    pthread_mutex_unlock(&push_mutex);

    return retval;
}

