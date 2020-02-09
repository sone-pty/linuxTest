#include "eventloopThread.h"

namespace sone
{
	eventloopThread::eventloopThread(const std::string& name)
		:_name(name), cond(_mutex) {}

	eventloopThread::~eventloopThread()
	{
		if(_loop)
		{
			_loop->exit();
			_thread->join();
		}
	}

	eventloop* eventloopThread::start()
	{
		_thread.reset(new Thread(_name, std::bind(&eventloopThread::threadfunc, this)));
		eventloop* l;
		{
			MutexLock::Lock lock(_mutex);
			while(!_loop)
				cond.wait();
			l = _loop;
		}
		return l;
	}

	void eventloopThread::threadfunc()
	{
		eventloop loop;
		{
			MutexLock::Lock lock(_mutex);
			_loop = &loop;
			cond.notify();
		}
		loop.startloop();
		_loop = nullptr;
	}
}
