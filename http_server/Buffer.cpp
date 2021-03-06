#include "Buffer.h"
#include <unistd.h>
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

	int Buffer::read(int fd, size_t len)
	{
		int n;
		if(len > (size_t)writeLen())
			_vec.reserve(_vec.capacity() + (len << 1));
		n = ::read(fd, tail(), len);
		high += n;
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
				_vec.reserve(_vec.capacity() + (len << 1));
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

	void Buffer::append(const std::string& message)
	{
		append(message.c_str(), message.length());
	}
	
	char* Buffer::peek()
	{
		return begin() + low;
	}

	char* Buffer::tail()
	{
		return begin() + high;
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
				return p - peek();
			else
				return -1;
		}
		else
			return -1;
	}

	ssize_t Buffer::findChar(char c, size_t begin, size_t end)
	{
		//assert(begin >= 0 && end <= dataLen() && begin < end);
		if (begin >= end || begin < 0 || end > dataLen())
			return -1;
		char *p = std::find(peek() + begin, peek() + end, c);
		if(p != peek() + end)
			return p - peek();
		else
			return -1;
	}

	bool Buffer::moveLow(size_t n)
	{
		if(n + low > high)
			return false;
		else
			low += n;
		return true;
	}

	bool Buffer::moveHigh(size_t n)
	{
		if(low > n)
			return false;
		high = n;
		return true;
	}
}
