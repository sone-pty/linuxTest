#include "epoller.h"
#include "dispatcher.h"

namespace sone
{
	epoller::epoller(eventloop* loop)
		:owner(loop), events(EVENTS_INIT_SIZE)
	{
		epollfd = ::epoll_create(5);
		if(epollfd < 0)
		{
			SONE_LOG_ROOT() << "epoll_create失败,epollfd < 0";
			abort();
		}
	}

	epoller::~epoller()
	{
		::close(epollfd);
	}
	
	util::Timestamp epoller::epoll(int timeout, std::vector<dispatcher*>* dl)
	{
		int ret = ::epoll_wait(epollfd, &*events.begin(), events.size(), timeout);
		int err = errno;
		util::Timestamp now;

		if(ret > 0)
		{
			//填充传入的向量
			for(int i = 0;i < ret;++i)
			{
				dispatcher* dis = static_cast<dispatcher*>(events[i].data.ptr);
				auto iter = dispatcher_map.find(dis->getFd());
				if(iter != dispatcher_map.end())
				{
					dis->setRevents(events[i].events);
					dl->push_back(dis);
				}
			}
			//如果事件量过大，扩大events向量
			if(ret > static_cast<int>(events.size()))
				events.resize(2 * events.size());
		}
		else if(ret == 0)
		{
			SONE_LOG_TRACE() << "epoller::epoll(): no events happened";
		}
		else
		{
			if(err != EINTR)
			{
				SONE_LOG_ERR() << "epoller::epoll() failed";
				abort();
			}
		}

		return now;
	}

	void epoller::setDispatcher(dispatcher* ed)
	{
		int fd = ed->getFd();
		auto iter = dispatcher_map.find(fd);
		//新的dispatcher
		if(iter == dispatcher_map.end())
		{
			dispatcher_map[fd] = ed;
			updateEpoller(EPOLL_CTL_ADD, ed);
		}
		//修改现有的dispatcher
		else
		{
			updateEpoller(EPOLL_CTL_MOD, ed);
		}
	}

	void epoller::delDispatcher(dispatcher* ed)
	{
		int fd = ed->getFd();
		if(dispatcher_map.find(fd) != dispatcher_map.end())
		{
			dispatcher_map.erase(fd);
			updateEpoller(EPOLL_CTL_DEL, ed);
		}
		else
			SONE_LOG_ERR() << "delDispatcher(): no specifed member in the dispatcher_map";
	}

	void epoller::updateEpoller(int operation, dispatcher* ed)
	{
		struct epoll_event e;
		memset(&e, 0, sizeof(e));
		e.events = ed->getEvents();
		e.data.ptr = ed;
		std::string tip;

		switch(operation)
		{
			case EPOLL_CTL_ADD:
				tip = "EPOLL_CTL_ADD";break;
			case EPOLL_CTL_DEL:
				tip = "EPOLL_CTL_DEL";break;
			case EPOLL_CTL_MOD:
				tip = "EPOLL_CTL_MOD";break;
			default:
				tip = "error_code for EPOLL_CTL_XXX";break;
		}

		if(::epoll_ctl(epollfd, operation, ed->getFd(), &e) < 0)
		{
			//记录错误日志并结束进程
			SONE_LOG_ERR() << "::epoll_ctl() && operation: " << tip;
			abort();
		}
	}
}
