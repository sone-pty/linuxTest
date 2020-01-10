#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>
#include <stdexcept>
#include <exception>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

namespace m_utils
{
	//判断一个数是否为素数
	bool isPrime(unsigned int v);
	//返回给定值的下一个素数
	unsigned int nextPrime(unsigned int n);
	//设置阻塞sokcet
	int setNonBlockingSoc(int fd);
	//非阻塞connect，如果成功调用返回socket；失败则返回-1
	int unblockConnect(const char* ip, short port, int times);
}

#endif
