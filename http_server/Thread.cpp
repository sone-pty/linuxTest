#include "Thread.h"

namespace sone
{
	static thread_local std::string thread_name = "UNKNOW";
	static thread_local Thread* thread_ptr = nullptr;

	Thread::Thread(const std::string& name, const std::function<void()>& func)
		:m_name(name), m_func(func)
	{
		if(name.empty())
			m_name = "UNKNOW";
		if(pthread_create(&m_thread, nullptr, &Thread::run, this))
		{
			//处理错误，输出日志
		}
		//确保对象创建完成时，线程已经启动
		m_sem.wait();
	}

	Thread::~Thread()
	{
		//确保回收线程资源
		if(m_thread)
			pthread_detach(m_thread);
	}

	const std::string& Thread::getName()
	{
		return m_name;
	}

	pid_t Thread::getThreadId()
	{
		return m_threadId;
	}

	void Thread::join()
	{
		if(m_thread)
		{
			if(pthread_join(m_thread, nullptr))
			{
				//错误处理
			}
			//确保析构的时候不会调用pthread_detach
			m_thread = 0;
		}
	}

	void* Thread::run(void* arg)
	{
		Thread* t = static_cast<Thread*>(arg);
		thread_ptr = t;
		thread_name = t->getName();
		t->m_threadId = gettid();
		pthread_setname_np(pthread_self(), t->m_name.substr(0, 15).c_str());
		t->m_sem.notify();
		t->m_func();
		return nullptr;
	}

	const std::string& Thread::GetName()
	{
		return thread_name;
	}

	void Thread::setName(const std::string& name)
	{
		if(name.empty())
			return;
		this->m_name = name;
		thread_name = name;
	}

	void Thread::SetName(const std::string& name)
	{
		if(name.empty())
			return;
		if(thread_ptr)
		{
			thread_ptr->m_name = name;
			thread_name = name;
		}
	}

	Thread* Thread::GetThis()
	{
		return thread_ptr;
	}
}
