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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include <algorithm>

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
//通过fd解析地址
struct sockaddr_in getAddrbyFdV4(int fd);
//URL解码
std::string URLDecode(const std::string& s);
//比较两个GMT字符串的大小(如果s1 < s2返回false；否则返回true)
bool compareGMTStr(const std::string &s1, const std::string &s2);

//时间戳类(us级别)
class Timestamp{
public:
	typedef std::shared_ptr<Timestamp> ptr;
	//默认构造函数，构造当前时间
	explicit Timestamp();
	//指定结构体
	explicit Timestamp(struct timeval& tv);
	explicit Timestamp(time_t s, int64_t us);
	//返回秒级别的时间戳(usec为true时返回微妙级别)
	std::string to_string(bool usec);
	std::string toGMTString();
	void setSecond(time_t sec) { this->sec = sec; }
	void setUSecond(int64_t usec) { this->usec = usec; }
	time_t getSecond() const { return sec; }
	int64_t getUSecond() const { return usec; }
private:
	bool now();
private:
	struct timeval _tv;
	time_t sec;
	int64_t usec;
};

}

#endif
