/*
 * coltthread.cpp
 *
 *  Created on: Sep 5, 2014
 *      Author: paul
 */

#include "colt_headers.h"
#include <thread>
#include "concurrentqueue.h"
#include "coltthread.h"

colt_thread::colt_thread(colt_base &in):
	colt_operator(in),
	rec_nums(),
	local_thread(NULL)
{
	i_am = colt_class_thread;
}

colt_thread::~colt_thread() {
	// TODO Auto-generated destructor stub
}

int colt_thread::preprocess()
{
//	struct F f;
//	std::thread t(f);
	local_routine.immediate = this;
	local_thread = new std::thread(local_routine);

	return colt_operator::preprocess();
}

int colt_thread::process(int rec_num)
{
	rec_nums.push(rec_num);
	return 1;
}

void colt_thread::postprocess()
{
	rec_nums.push(-1);
	local_thread->join();
}
