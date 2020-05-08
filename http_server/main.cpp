#include <signal.h>
#include "../sysHeader.h"
#include "eventloop.h"
#include "HttpServer.h"
#include "consts.h"
#include "TimerHeap.h"

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

TimerHeap* timerheap = new TimerHeap();

static void sig_pipe(int) {}

static void sig_alarm(int)
{
	timerheap->tick();
	alarm(EXPIRE_TIME);
}

int main(void)
{
	//设置信号处理函数
	signal(SIGALRM, sig_alarm);
	signal(SIGPIPE, sig_pipe);
	alarm(EXPIRE_TIME);
	//启动服务器
	//InetAddress addr("127.0.0.1", 8987, false);
	InetAddress addr("172.17.0.16", 8987, false);
	eventloop loop;
	HttpServer server(&loop, addr, timerheap);
	server.start();
	loop.startloop();

	return 0;
}
