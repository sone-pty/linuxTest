#ifndef UTIL_H_
#define UTIL_H_

#include <stdexcept>
#include <exception>
#include <syscall.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#include "Log.h"
#include <string.h>
#include <memory>

#define BYTE_NUMS 8
#define BUFFSIZE 4096

typedef int pid_t;

//字节序
#define BYTEORDER_BIG 1
#define BYTEORDER_LITTLE 0

namespace util
{

//判断主机的字节序，如果返回值大于0表示大端序，如果等于零表示小端序；-1表示错误
int byteorder();
//返回线程id
pid_t getTid();

//时间戳类(us级别)
class Timestamp{
public:
	typedef std::shared_ptr<Timestamp> ptr;
	//默认构造函数，构造当前时间
	explicit Timestamp();
	//指定结构体
	explicit Timestamp(struct timeval& tv);
	//返回秒级别的时间戳(usec为true时返回微妙级别)
	std::string to_string(bool usec);
private:
	bool now();
private:
	struct timeval _tv;
	time_t sec;
	int64_t usec;
};

}

#endif
