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
#include "InetAddress.h"
#include "Socket.h"
#include "eventloop.h"
#include "HttpServer.h"
#include "http.h"

using namespace std;
using namespace sone;
using namespace util;

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
class obj{
public:
	void setF(function<void()> f)
	{
		this->f = move(f);
	}

	void execute()
	{
		f();
	}

private:
	function<void()> f;
};

class parent : public std::enable_shared_from_this<parent>
{
public:
	parent(obj& o)
	{
		o.setF(bind(&parent::func, this));
	}
	virtual void func()
	{
		cout << "parent::func" << endl;
	}
	virtual void func2()
	{
		cout << "parent::func2" << endl;
	}
};

void way(shared_ptr<parent> ptr)
{
	ptr->func2();
}

class child : public parent{
public:
	child(obj& o):parent(o)
	{

	}
	void func()
	{
		cout << "child::func" << endl;
		way(shared_from_this());
	}

	void func2()
	{
		cout << "child::func2" << endl;
	}
};

int main(void)
{
	// InetAddress addr("127.0.0.1", 8987, false);
	// eventloop loop;
	// HttpServer server(&loop, addr);
	// server.start();
	// loop.startloop();
	return 0;
}
