/*
 * coltsync.h
 *
 *  Created on: Sep 7, 2014
 *      Author: paul
 */

#ifndef COLTSYNC_H_
#define COLTSYNC_H_

#include <pthread.h>

class colt_sync : public colt_operator {
	pthread_mutex_t push_mutex;
public:
	colt_sync(colt_base &in);
	virtual ~colt_sync();

	int process(int rec_num);
};

#endif /* COLTSYNC_H_ */
