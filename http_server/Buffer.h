#ifndef BUFFER_H_
#define BUFFER_H_

#include <vector>
#include <string>

namespace sone
{

/*
 * [low, high)表示有效数据部分；[high, _vec.size)表示可写部分；空闲部分等于可写部分加上[0, low)
 * */
class Buffer{
public:
	Buffer();
	~Buffer();
	//从对应fd中读取数据
	int read(int fd);
	//返回此buffer在内存中的首地址（不一定是数据首地址）
	char* begin();
	//返回此buffer数据的首地址
	char* peek();
	//从此buffer中的指定位置（默认从peek开始）读取指定字节的数据，并转为string
	std::string getDataToString(size_t n, size_t pos = 0);
	//从指定位置(pos表示距离peek()的距离)开始找到第一个出现的字符c，返回其距离peek()的长度;找不到则返回-1
	ssize_t findChar(char c, size_t pos = 0);
	//范围查找[begin, end)；返回其距离peek()的长度；找不到返回-1
	ssize_t findChar(char c, size_t begin, size_t end);
	//从此buffer中读取出n个字节（将low后移）
	bool moveLow(size_t n);
	//可能需要扩容
	void append(const char* buf, size_t len);
	void append(const std::string& message);
	//当前空闲部分长度
	int freeLen() { return low + _vec.size() - high; }
	//当前有效数据长度
	int dataLen() { return high - low; }
	//当前可写部分长度
	int writeLen() { return _vec.size() - high; }
	//当前buffer的容量
	size_t capcity() const { return _vec.capacity();  }
private:
	std::vector<char> _vec;
	int low;
	int high;
};

}

#endif
