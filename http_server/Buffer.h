#ifndef BUFFER_H_
#define BUFFER_H_

#include <vector>

namespace sone
{

/*
 * [low, high)表示有效数据部分；[high, _vec.size)表示可写部分；空闲部分等于可写部分加上[0, low)
 * */
class Buffer{
public:
	Buffer();
	~Buffer();
	int read(int fd);
	char* begin();
	//可能需要扩容
	void append(char* buf, size_t len);
	//当前空闲部分长度
	int freeLen() { return low + _vec.size() - high; }
	//当前有效数据长度
	int dataLen() { return high - low; }
	//当前可写部分长度
	int writeLen() { return _vec.size() - high; }
private:
	std::vector<char> _vec;
	int low;
	int high;
};

}

#endif
