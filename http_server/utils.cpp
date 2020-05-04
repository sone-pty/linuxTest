#include "utils.h"
#include <regex>
#include <map>

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
		res.push_back('\0');
		return std::string(&(*res.begin()));
	}

	struct Icase : public std::binary_function<std::string, std::string, bool>
	{
		bool operator()(const std::string& lhs, const std::string& rhs) const
		{
			return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
		}
	};
	
	static std::map<std::string, int, Icase> MONTH_MAP = 
	{
		{ "Jan", 1 },
		{ "Feb", 2 },
		{ "Mar", 3 },
		{ "Apr", 4 },
		{ "May", 5 },
		{ "Jun", 6 },
		{ "Jul", 7 },
		{ "Aug", 8 },
		{ "Sept", 9 },
		{ "Oct", 10 },
		{ "Nov", 11 },
		{ "Dec", 12 },
	};
	
	bool compareGMTStr(const std::string &s1, const std::string &s2)
	{
		struct date{
			int year, month, day;
			int hour, minute, sec;
		}d1, d2;
		
		std::regex r("[a-z]{3,3}, ([0-9]{1,2}) ([a-z]{3,4}) ([0-9]{4,4}) ([0-9][0-9]):([0-9][0-9]):([0-9][0-9]) GMT", std::regex::icase);
		std::smatch res;
		
		if(std::regex_search(s1, res, r))
		{
			d1.day = atoi(res.str(1).c_str());
			d1.month = MONTH_MAP[res.str(2)];
			d1.year = atoi(res.str(3).c_str());
			d1.hour = atoi(res.str(4).c_str());
			d1.minute = atoi(res.str(5).c_str());
			d1.sec = atoi(res.str(6).c_str());
		}
		if(std::regex_search(s2, res, r))
		{
			d2.day = atoi(res.str(1).c_str());
			d2.month = MONTH_MAP[res.str(2)];
			d2.year = atoi(res.str(3).c_str());
			d2.hour = atoi(res.str(4).c_str());
			d2.minute = atoi(res.str(5).c_str());
			d2.sec = atoi(res.str(6).c_str());
		}
		
		if(d1.year > d2.year)
			return true;
		else if(d1.year < d2.year)
			return false;
		else if(d1.month > d2.month)
			return true;
		else if(d1.month < d2.month)
			return false;
		else if(d1.day > d2.day)
			return true;
		else if(d1.day < d2.day)
			return false;
		else if(d1.hour > d2.hour)
			return true;
		else if(d1.hour < d2.hour)
			return false;
		else if(d1.minute > d2.minute)
			return true;
		else if(d1.minute < d2.minute)
			return false;
		else if(d1.sec >= d2.sec)
			return true;
		else
			return false;
	}

	/*Timestamp*/
	Timestamp::Timestamp()
	{
		memset(&_tv, 0, sizeof(struct timeval));
		if(!now())
			SONE_LOG_ROOT() << "Timestamp获取当前时间失败";
	}

	Timestamp::Timestamp(time_t s, int64_t us)
		:sec(s), usec(us) {}

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
		::strftime(s, 128, "%a, %d %b %Y %H:%M:%S GMT", &t);
		return std::string(s);
	}
}
