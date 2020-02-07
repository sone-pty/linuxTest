#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include <functional>
#include "mutex.h"
#include "utils.h"

typedef int pid_t;

namespace sone
{

class dispatcher;
class epoller;

//事件循环类
class eventloop{
public:
	typedef std::shared_ptr<eventloop> ptr;
	typedef std::function<void()> functor;

	eventloop();
	~eventloop();
	//开启循环
	void startloop();
	//更新dispatcher或者添加新的dispatcher
	void updateDispatcher(dispatcher* ed);
	//删除dispatcher
	void removeDispatcher(dispatcher* ed);
	//epoll_wait的返回时间
	util::Timestamp getEpollTime() { return ret_time; }
	//将调用加入到事件循环中处理
	void runInLoop(functor f);
	//结束事件循环
	void exit();
private:
	//处理wakeupfd的读事件
	void process_wakeup_read();
private:
	//每轮事件循环触发的各个fd对应的事件处理派生器
	std::vector<dispatcher*> active_dispatchers;
	//每轮事件循环的返回时间
	util::Timestamp ret_time;
	//IO复用器
	std::unique_ptr<epoller> _epoller;
	//状态
	volatile bool quit;
	//非事件处理过程集合(即非IO线程产生的事件处理)
	std::vector<functor> non_io_functors;
	//保护上面向量的锁
	MutexLock _mutex;
	//用来唤醒可能阻塞的当前IO线程
	int wakeupfd;
	//对应于wakeupfd的dispatcher
	std::unique_ptr<dispatcher> wakeup_dispatcher;
	//创建者线程ID
	pid_t threadId;
};

}

#endif
