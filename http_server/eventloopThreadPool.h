#ifndef EVENTLOOPTHREADPOOL_H_
#define EVENTLOOPTHREADPOOL_H_

#include "nocopyable.h"
#include "eventloopThread.h"

namespace sone
{
	
class eventloopThreadPool : nocopyable{
public:
	explicit eventloopThreadPool(int threadNums);
	void start();
	eventloop* getLoop(int fd);
private:
	int _threadNums;
	std::vector<std::unique_ptr<eventloopThread>> _threads;
	std::vector<eventloop*> _loops;
};

}

#endif
