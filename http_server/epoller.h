#ifndef EPOLLER_H_
#define EPOLLER_H_

#include "utils.h"
#include <unordered_map>
#include <sys/epoll.h>

namespace sone
{

class dispatcher;
class eventloop;

//IO复用器(对epoll进行封装，只使用LT模式)
class epoller{
public:
	epoller(eventloop* loop);
	~epoller();
	//IO复用函数(timeout是超时时间，el在调用时传入，表示待填充的有效的fd对应的处理派生器集合)
	util::Timestamp epoll(int timeout, std::vector<dispatcher*>* dl);
	//更新或者添加一个dispatcher
	void setDispatcher(dispatcher* ed);
	//删除一个dispatcher
	void delDispatcher(dispatcher* ed);
private:
	void updateEpoller(int operation, dispatcher* ed);
private:
	const static int EVENTS_INIT_SIZE = 16;
	//内核事件表描述符
	int epollfd;
	//fd->dispatcher的映射
	std::unordered_map<int, dispatcher*> dispatcher_map;
	//所属于的eventloop
	eventloop* owner;
	//epoll_wait的待填充向量
	std::vector<struct epoll_event> events;
};

}

#endif
