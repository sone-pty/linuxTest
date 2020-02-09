#include "eventloopThreadPool.h"

namespace sone
{
	eventloopThreadPool::eventloopThreadPool(int threadNums)
		:_threadNums(threadNums) {}

	void eventloopThreadPool::start()
	{
		char name[32];
		for(int i = 0; i < _threadNums; ++i)
		{
			sprintf(name, "Work-Thread#%d", i);
			auto t = std::unique_ptr<eventloopThread>(new eventloopThread(name));
			_threads.push_back(t);
			_loops.push_back(t->start());
		}
	}

	eventloop* eventloopThreadPool::getLoop(int fd)
	{
		if(!_loops.empty())
			return _loops[fd % _threadNums];
		else
			return nullptr;
	}
}
