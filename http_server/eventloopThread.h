#ifndef EVENTLOOPTHREAD_H_
#define EVENTLOOPTHREAD_H_

#include "eventloop.h"
#include "Thread.h"
#include "condition.h"
#include "mutex.h"
#include "nocopyable.h"

namespace sone
{

class eventloopThread : nocopyable{
public:
	explicit eventloopThread(const std::string& name);
	~eventloopThread();
	eventloop* start();
private:
	void threadfunc();
private:
	eventloop* _loop;
	Thread::ptr _thread;
	std::string _name;
	MutexLock _mutex;
	Condition cond;
};

}

#endif
