/*
 * coltthread.h
 *
 *  Created on: Sep 5, 2014
 *      Author: paul
 */

#ifndef COLTTHREAD_H_
#define COLTTHREAD_H_

class colt_thread : public colt_operator {
	concurrent_queue<int>	rec_nums;
	struct F
	{
		colt_thread	*immediate;
	  void operator() () const
	  {
	    int rec_num;
	    while(immediate->rec_nums.pop(&rec_num)) {\
	    	if(rec_num == -1)
	    		return;
	    	immediate->out_object->process(rec_num);
	    }
	  }
	};
	struct F local_routine;
	std::thread *local_thread;

public:
	colt_thread(colt_base &in);
	virtual ~colt_thread();

	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTTHREAD_H_ */
