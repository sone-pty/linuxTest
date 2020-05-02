#ifndef TIMERHEAP_H_
#define TIMERHEAP_H_

#include <vector>
#include "mutex.h"
#include <time.h>
#include <functional>

namespace sone
{

class Timer{
public:
	//定时器过期的相对时间, 以秒为单位
	Timer(size_t sec)
		:expire(time(nullptr) + sec) {}
	time_t getExpire() const { return expire; }
	std::function<void()> getCb() const { return _cb; }
	void setCb(std::function<void()> cb) { this->_cb = std::move(cb); }
private:
	//过期时间
	time_t expire;
	//回调
	std::function<void()> _cb;	
};

class TimerHeap{
public:
	typedef std::unique_ptr<TimerHeap> ptr;

	TimerHeap(size_t capcity = 64);
	~TimerHeap();
	void pushTimer(Timer* timer);
	void popTimer();
	Timer* topTimer();
	void tick();
	bool empty();
private:
	//构建二叉堆的数组
	std::vector<Timer*> _vec;
	//锁
	MutexLock _lock;
	//堆中元素个数
	size_t _nums;
};

}

#endif
