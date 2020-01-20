#ifndef UTIL_H_
#define UTIL_H_

#include <stdexcept>
#include <exception>


#define BYTE_NUMS 8

//字节序
#define BYTEORDER_BIG 1
#define BYTEORDER_LITTLE 0

namespace util
{
	//判断主机的字节序，如果返回值大于0表示大端序，如果等于零表示小端序；-1表示错误
	int byteorder();
}

#endif
