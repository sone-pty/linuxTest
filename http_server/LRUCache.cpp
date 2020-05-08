#include "LRUCache.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

namespace sone
{

	const char* LRUCache::getValue(const std::string& k)
	{
		MutexLock::Lock l(_lock);
		bool flag = false;
		if(cacheMap.find(k) == cacheMap.end())
			return nullptr;
		//将最近访问的节点移到链表头部
		cacheList.splice(cacheList.begin(), cacheList, cacheMap[k]);
		//更新哈希表缓存
		cacheMap[k] = cacheList.begin();
		//查看缓存是否过期
		struct stat s;
		if(stat(k.c_str(), &s) < 0)
			return nullptr;
		if(s.st_mtim.tv_sec > cacheMap[k]->mtime.tv_sec)
			flag = true;
		else if(s.st_mtim.tv_nsec > cacheMap[k]->mtime.tv_nsec)
			flag = true;
		//缓存未过期
		if(!flag)
			return cacheMap[k]->val;
		else
		{
			delete cacheMap[k]->val;
			char* buf = new char[s.st_size];
			int fd = ::open(k.c_str(), O_RDONLY);
			auto sz = ::read(fd, buf, s.st_size);
			assert(sz == s.st_size);
			cacheMap[k]->val = buf;
			cacheMap[k]->mtime = s.st_mtim;
			cacheMap[k]->fsize = sz;
			return buf;
		}
	}

	void LRUCache::setValue(const std::string& k)
	{
		MutexLock::Lock l(_lock);
		if(cacheMap.find(k) == cacheMap.end())
		{
			//缓存满了，弹出末尾节点
			if(this->capacity == cacheList.size())
			{
				cacheMap.erase(cacheList.back().key);
				delete cacheList.back().val;
				cacheList.back().val = nullptr;
				cacheList.pop_back();
			}
			//插入新节点
			CacheNode node;
			struct stat s;
			int ret = stat(k.c_str(), &s);
			assert(ret == 0);
			char* buf = new char[s.st_size];
			int fd = ::open(k.c_str(), O_RDONLY);
			auto sz = ::read(fd, buf, s.st_size);
			assert(sz == s.st_size);
			node.key = k;
			node.val = buf;
			node.mtime = s.st_mtim;
			node.fsize = sz;
			cacheList.push_front(node);
			cacheMap[k] = cacheList.begin();
		}
	}

	size_t LRUCache::getSize(const std::string& k)
	{
		if(cacheMap.find(k) == cacheMap.end())
			return 0;
		else
			return cacheMap[k]->fsize;
	}

	bool LRUCache::isExist(const std::string& k)
	{
		MutexLock::Lock l(_lock);
		if(cacheMap.find(k) == cacheMap.end())
			return false;
		else
			return true;
	}
}
