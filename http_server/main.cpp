#include <iostream>
#include <signal.h>
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
#include "Buffer.h"

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

void handle_pipe(int sig) {}

int main(void)
{
	//忽视SIGPIPE
	struct sigaction sa;
	sa.sa_handler = handle_pipe;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);

	InetAddress addr("127.0.0.1", 8987, false);
	eventloop loop;
	HttpServer server(&loop, addr);
	server.start();
	loop.startloop();
	return 0;
}