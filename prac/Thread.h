#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include <string>
#include <unistd.h>
#include <functional>
#include <memory>
#include "nocopyable.h"
#include "mutex.h"
#include <syscall.h>

#define gettid() syscall(SYS_gettid)

namespace sone
{

class Thread: nocopyable{
public:
	typedef std::shared_ptr<Thread> ptr;
	
	Thread(const std::string& name, const std::function<void()>& func);
	~Thread();
	pid_t getThreadId();
	const std::string& getName();
	void setName(const std::string&);
	void join();
	
	//以下仅仅对Thread类生成的线程有效
	static const std::string& GetName();
	static void SetName(const std::string& name);
	static Thread* GetThis();
private:
	static void* run(void*);
private:
	//线程标识
	pthread_t m_thread;
	//线程ID
	pid_t m_threadId;
	//线程名
	std::string m_name;
	//信号量
	Semaphore m_sem;
	//执行函数
	std::function<void()> m_func;
};

}

#endif
