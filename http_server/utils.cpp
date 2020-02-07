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
		std::string res;
		::localtime_r(&sec, &t);
		::strftime(s, 128, "%Y-%m-%d %H:%M:%S", &t);
		if(usec)
			sprintf(s + strlen(s), ":%06ld", this->usec);
		return std::string(s);
	}
}
