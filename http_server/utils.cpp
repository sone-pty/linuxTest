#include "utils.h"

namespace util
{
	//线程ID缓存
	thread_local pid_t cache_tid = -1;
	
	pid_t getTid()
	{
		if(cache_tid != -1)
			return cache_tid;
		cache_tid = syscall(SYS_gettid);
		return cache_tid;
	}
	
	int byteorder()
	{
		union
		{
			short s;
			char bytes[sizeof(short)];
		}t;

		t.s = 0x0102;

		if(t.bytes[0] == 1 && t.bytes[1] == 2)
			return BYTEORDER_BIG;
		else if(t.bytes[0] == 2 && t.bytes[1] == 1)
			return BYTEORDER_LITTLE;
		else
			return -1;
	}

	struct sockaddr_in getAddrbyFdV4(int fd)
	{
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		socklen_t len = sizeof(addr);
		if(::getsockname(fd, (sockaddr*)&addr, &len) < 0)
			SONE_LOG_ERR() << "getsockname() failed";
		return addr;
	}

	static std::map<char, int> HEXMAP =
	{
		{ '1', 1 },
		{ '2', 2 },
		{ '3', 3 },
		{ '4', 4 },
		{ '5', 5 },
		{ '6', 6 },
		{ '7', 7 },
		{ '8', 8 },
		{ '9', 9 },
		{ 'a', 10 },
		{ 'b', 11 },
		{ 'c', 12 },
		{ 'd', 13 },
		{ 'e', 14 },
		{ 'f', 15 },
		{ 'A', 10 },
		{ 'B', 11 },
		{ 'C', 12 },
		{ 'D', 13 },
		{ 'E', 14 },
		{ 'F', 15 },
	};

	std::string URLDecode(const std::string& s)
	{
		std::vector<char> res;
		res.reserve(s.size());

		for (int i = 0; i < (int)s.size();)
		{
			if(s[i] != '%')
				res.push_back(s[i++]);
			else if(i + 2 >= (int)s.size())
				return nullptr;
			else
			{
				int n = HEXMAP[s[i + 1]] * 16 + HEXMAP[s[i + 2]];
				res.push_back(n);
				i += 3;
			}
		}

		return (&(*res.begin()));
	}

	/*Timestamp*/
	Timestamp::Timestamp()
	{
		memset(&_tv, 0, sizeof(struct timeval));
		if(!now())
			SONE_LOG_ROOT() << "Timestamp获取当前时间失败";
	}

	Timestamp::Timestamp(struct timeval& tv)
		:_tv(tv)
	{
		memset(&_tv, 0, sizeof(struct timeval));
		if(!now())
			SONE_LOG_ROOT() << "Timestamp获取当前时间失败";
	}

	bool Timestamp::now()
	{
		int ret = ::gettimeofday(&_tv, nullptr);
		sec = static_cast<time_t>(_tv.tv_sec);
		usec = _tv.tv_usec;
		if(ret < 0)
			return false;
		else
			return true;
	}
	
	std::string Timestamp::to_string(bool usec)
	{
		struct tm t;
		char s[128];
		::localtime_r(&sec, &t);
		::strftime(s, 128, "%Y-%m-%d %H:%M:%S", &t);
		if(usec)
			sprintf(s + strlen(s), ":%06ld", this->usec);
		return std::string(s);
	}

	std::string Timestamp::toGMTString()
	{
		struct tm t;
		char s[128];
		::localtime_r(&sec, &t);
		::strftime(s, 128, "%a, %d %b %Y %H:%M:%S", &t);
		return std::string(s);
	}
}
