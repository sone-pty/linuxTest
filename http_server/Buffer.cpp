#include "Buffer.h"
#include <sys/uio.h>
#include <algorithm>

namespace sone
{
	Buffer::Buffer()
		:_vec(4096, 0), low(0), high(0)
	{

	}
	
	Buffer::~Buffer()
	{

	}

	char* Buffer::begin()
	{
		return &(*_vec.begin());
	}

	int Buffer::read(int fd)
	{
		char extrabuf[65536];
		struct iovec iov[2];
		int len = writeLen();

		iov[0].iov_base = begin() + high;
		iov[0].iov_len = static_cast<size_t>(len);
		iov[1].iov_base = extrabuf;
		iov[1].iov_len = 65536;

		ssize_t n = ::readv(fd, iov, 2);
		if(n < 0)
			return -1;
		else if(n <= len)
		{
			high += n;
		}
		else
		{
			high = _vec.size();
			append(extrabuf, n - len);
		}

		return n;
	}

	void Buffer::append(const char* buf, size_t len)
	{
		if(len == 0)
			return;
		int writeable = writeLen();
		if(len <= static_cast<size_t>(writeable))
		{
			std::copy(buf, buf + len, begin() + high);
			high += len;
		}
		else
		{
			//空闲部分不足以填入，需要扩容
			if(static_cast<size_t>(freeLen()) < len)
			{
				_vec.resize(2 * _vec.size());
				std::copy(buf, buf + len, begin() + high);
				high += len;
			}
			//空闲部分足够，直接对数据进行紧凑
			else
			{
				std::copy(begin() + low, begin() + high, begin());
				low = 0;
				high = dataLen();
				std::copy(buf, buf + len, begin() + high);
				high += len;
			}
		}
	}

	char* Buffer::peek()
	{
		return begin() + low;
	}
	
	std::string Buffer::getDataToString(size_t n, size_t pos)
	{
		char* p = peek() + pos;
		if(n > static_cast<size_t>(dataLen() - pos))
			return nullptr;
		else
			return std::string(p, p + n);
	}

	ssize_t Buffer::findChar(char c, size_t pos)
	{
		char* p;
		size_t len = static_cast<size_t>(dataLen());
		
		if(pos < len)
		{
			p = std::find(peek() + pos, peek() + len, c);
			if(p != peek() + len)
				return p - peek() - pos;
			else
				return -1;
		}
		else
			return -1;
	}
}
