#ifndef LRUCACHE_H_
#define LRUCACHE_H_

#include <unordered_map>
#include <list>
#include <string>
#include "mutex.h"

namespace sone
{
	
class CacheNode{
public:
	CacheNode()
		:key(""), val(nullptr) {}
	CacheNode(const std::string& k, char* v, struct timespec mt)
		:key(k), val(v), mtime(mt) {}
	std::string key;
	char* val;
	struct timespec mtime;
	size_t fsize;
};

class LRUCache{
public:
	LRUCache(size_t cap)
		:capacity(cap) {}
	const char* getValue(const std::string& k);
	size_t getSize(const std::string& k);
	void setValue(const std::string& k);
	bool isExist(const std::string& k);
private:
	std::list<CacheNode> cacheList;
	std::unordered_map<std::string, std::list<CacheNode>::iterator> cacheMap;
	size_t capacity;
	MutexLock _lock;
};

}

#endif
