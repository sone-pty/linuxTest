#include <iostream>
#include "Log.h"
#include "mutex.h"
#include "condition.h"
#include <memory>
#include <functional>
#include "Thread.h"
#include "nocopyable.h"
#include <atomic>
#include <vector>
#include <bitset>
#include "utils.h"
#include "../sysHeader.h"

using namespace std;
/*
void dynamic_arg_func(const char* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	char* buf;
	int len = vasprintf(&buf, fmt, vl);

	if(len != -1)
		std::cout << buf << std::endl;
	free(buf);
	va_end(vl);
}
*/

sone::MutexLock m_lock;
sone::SpinLock s_lock;
sone::Condition cond(m_lock);
static volatile int count = 0;
static volatile bool tern = false;
sone::Semaphore sem1, sem2;

void* run1(void*)
{
	sem1.wait();
	cout << "thread1 start" << endl;

	sone::ScopedLock<sone::MutexLock> a(m_lock);
	while(1)
	{
		sleep(1);
		while(!tern)
			cond.wait();
		cout << pthread_self() << ":" << ++count << endl;
		tern = !tern;
	}
	return nullptr;
}

void* run2(void*)
{
	sem2.wait();
	cout << "thread2 start" << endl;

	while(1)
	{
		sone::ScopedLock<sone::MutexLock> a(m_lock);
		if(!tern)
			cout << pthread_self() << ":" << ++count << endl;
		tern = !tern;
		a.unlock();
		cond.nofity();
		sleep(1);
	}
	return nullptr;
}

class parent{
public:
	parent() = default;
	parent(const string& s)
		:name(s)
	{

	}
	virtual ~parent() = default;
	parent(const parent& p)
		:name(p.name)
	{
		cout << "parent copy constructor" << endl;	
	}
private:
	string name;
};

class child: public parent{
public:
	child(int i, const string& n)
		:parent(n), index(i)
	{

	}
	~child() = default;
	child(const child& c)
		:parent(c), index(c.index)
	{	
		cout << "child copy constructor" << endl;
	}
private:
	int index;
};

class Object{
public:
	Object()
		:vec(1000, 0.0), a(0), b(0), c(0)
	{
		
	}

	Object& operator=(const Object& o)
	{
		vec = o.vec;
		a = o.a;
		b = o.b;
		c = o.c;
		return *this;
	}
private:
	vector<double> vec;
	int a, b, c;
};

class ObjectWrapper{
public:
	ObjectWrapper()
		:p(new Object())
	{

	}

	ObjectWrapper(const ObjectWrapper& o)
		:p(new Object(*o.p))
	{
		
	}

	ObjectWrapper& operator=(const ObjectWrapper& o)
	{
		if(this == &o)
			return *this;
		*p = *o.p;

		return *this;
	}
	
	void swap(ObjectWrapper& a)
	{
		auto temp = p;
		p = a.p;
		a.p = temp;
	}
private:
	Object* p;
};

namespace std
{
	template <> void swap<ObjectWrapper>(ObjectWrapper& a, ObjectWrapper& b)
	{
		a.swap(b);
	}
}

static std::atomic_flag flag = ATOMIC_FLAG_INIT;

sone::CASLock c_lock;
void run()
{
	cout << sone::Thread::GetName() << "start working!" << endl;
	sone::CASLock::Lock l(c_lock);
	for(int j = 0;j < 10000000;++j)
		++count;
	l.unlock();
	cout << sone::Thread::GetName() << "finish working!" << endl;
}

int main(void) {
	/*
    sone::Logger::ptr logger(new sone::Logger);
    logger->addAppender(sone::LogAppender::ptr(new sone::StdoutLogAppender));

    sone::FileLogAppender::ptr file_appender(new sone::FileLogAppender("./logfile.log"));
    sone::LogFormatter::ptr fmt(new sone::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sone::LogLevel::ERROR);

    logger->addAppender(file_appender);
	logger->setLevel(sone::LogLevel::FATAL);
    SONE_LOG_INFO(logger) << "test macro";
    SONE_LOG_ERROR(logger) << "test macro error";
    SONE_LOG_FMT_ERROR(logger, "test macro fmt error %s", "pty");
	*/
	/*
	vector<sone::Thread::ptr> vec;
	for(int i = 0;i < 10;++i)
		vec.push_back(sone::Thread::ptr(new sone::Thread("thread" + std::to_string(i), std::function<void()>(run))));
	clock_t start = clock();
	for(int i = 0;i < 10;++i)
		vec[i]->join();
	clock_t end = clock();
	cout << "time: " << end - start << endl;
	cout << count << endl;
	*/
	/*
	static const size_t BLOCK_SIZE = 4;
	struct iovec svec[BLOCK_SIZE];
	char* ss[] = { (char*)"abcd", (char*)"efgh", (char*)"ijklmn" , (char*)"opqrstuvwxyz" };

	for(size_t i = 0;i < BLOCK_SIZE;++i)
	{
		svec[i].iov_base = ss[i];
		svec[i].iov_len = strlen(ss[i]);
	}
	//写入的文件
	int fd;
	if((fd = open("./tempfile", O_RDWR | O_CREAT, 00666)) < 0)
		perror("open");
	size_t size;
	if((size = writev(fd, svec, BLOCK_SIZE)) < 0)
		perror("writev");
	cout << "write to tempfile " << size << endl;
	*/

	return 0;
}
