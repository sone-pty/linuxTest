#include "FixedThreadPool.h"

namespace sone
{
	FixedThreadPool::FixedThreadPool(const std::string name)
		:_mutex(), _full(_mutex), _empty(_mutex), _name(name), isRunning(false)
	{
		
	}

	FixedThreadPool::~FixedThreadPool()
	{
		if(isRunning)
			stop();
	}

	void FixedThreadPool::start(int threadnums)
	{
		isRunning = true;
		//预分配容量
		_threadobj.reserve(threadnums);
		//类成员作为funciton对象需要bind
		for(int i = 0;i < threadnums;++i)
			_threadobj.emplace_back(new Thread(_name + std::to_string(i), std::bind(&FixedThreadPool::thread_func, this)));
	}

	void FixedThreadPool::stop()
	{
		MutexLock::Lock l(_mutex);
		isRunning = false;
		//为了接下来能够顺利的join，需要唤醒等待在_empty上的所有线程
		_empty.notifyAll();
		l.unlock();
		//连接每一个线程，注意到_threadobj中存放的是unique_ptr，需要用引用
		for(auto& p : _threadobj)
			p->join();
	}

	void FixedThreadPool::thread_func()
	{
		while(isRunning)
		{
			std::function<void()> task = take();
			if(task)
				task();
		}
	}

	std::function<void()> FixedThreadPool::take()
	{
		MutexLock::Lock l(_mutex);
		std::function<void()> task = nullptr;
		//如果当前队列为空，等待；while是为了防止虚假唤醒
		while(_wq.empty() && isRunning)
		{
			_empty.wait();	
		}
		//千万记得要判断当前队列是否为空，因为stop方法也可能唤醒阻塞在take上的线程（这个时候队列中是没有元素的，如果不加判断，会出现问题）
		if(!_wq.empty())
		{
			task = _wq.front();
			_wq.pop();
			//唤醒有可能等待在_full上的生产者
			_full.notify();
		}
		
		return task;
	}

	void FixedThreadPool::addTask(std::function<void()> cb)
	{
		MutexLock::Lock l(_mutex);
		while(_wq.size() > maxQueSize && isRunning)
		{
			_full.wait();
		}
		_wq.push(cb);
		_empty.notify();
	}

	size_t FixedThreadPool::queSize()
	{
		MutexLock::Lock l(_mutex);
		return _wq.size();
	}

	void FixedThreadPool::setMaxQueSize(size_t size)
	{
		if(size <= 0)
			throw std::logic_error("size 必须大于0");
		MutexLock::Lock l(_mutex);
		this->maxQueSize = size;
	}
}
