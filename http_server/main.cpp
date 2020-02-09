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

int main(void) {
	InetAddress addr("127.0.0.1", 8987, false);
	eventloop loop;
	HttpServer server(&loop, addr);
	server.start();
	loop.startloop();
	return 0;
}
