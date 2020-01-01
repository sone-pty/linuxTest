#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>
#include <stdexcept>
#include <exception>

namespace m_utils
{
	//判断一个数是否为素数
	bool isPrime(unsigned int v);
	//返回给定值的下一个素数
	unsigned int nextPrime(unsigned int n);
}

#endif
