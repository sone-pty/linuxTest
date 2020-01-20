#ifndef FIXEDTHREADPOOL_H_
#define FIXEDTHREADPOOL_H_

#include "condition.h"
#include "mutex.h"
#include <deque>
#include "nocopyable.h"
#include "Thread.h"
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <stdexcept>
#include <queue>

#define DEFAULT_QUE_SIZE 50

namespace sone
{

class FixedThreadPool : nocopyable{
public:
	typedef std::shared_ptr<FixedThreadPool> ptr;

	explicit FixedThreadPool(const std::string name = "DefaultThreadPool");
	~FixedThreadPool();
	void setMaxQueSize(size_t size);
	void start(int threadnums);
	void stop();
	const std::string& Name() const { return _name; }
	//工作队列中现有的工作任务数
	size_t queSize();
	//向线程池中加入一个任务
	void addTask(std::function<void()> cb);
	//线程实际执行的函数
	void thread_func();
private:
	//从队列中取出一个任务
	std::function<void()> take();
private:
	//队列的互斥量
	MutexLock _mutex;
	//队列满的条件变量
	Condition _full;
	//队列空的条件变量
	Condition _empty;
	//线程池的名字
	std::string _name;
	//存放线程对象指针的向量
	std::vector<std::unique_ptr<Thread>> _threadobj;
	//工作队列
	std::queue<std::function<void()>> _wq;
	//线程池的工作状态
	volatile bool isRunning;
	//工作队列中最大允许的任务上限
	volatile size_t maxQueSize = DEFAULT_QUE_SIZE;
};

}

#endif
