#include "dispatcher.h"
#include "Log.h"
#include "eventloop.h"

namespace sone
{
	int dispatcher::getFd()
	{
		return _fd;
	}

	int dispatcher::getEvents()
	{
		return events;
	}

	void dispatcher::setRevents(int revents)
	{
		this->revents = revents;
	}

	void dispatcher::process()
	{
		if(revents & EPOLLRDHUP)
		{
			SONE_LOG_TRACE() << "fd = " << _fd << " trigger EPOLLRDHUP";
			close_callback();
		}
		else if(revents & EPOLLOUT)
		{
			write_callback();
		}
		else if(revents & EPOLLIN)
		{
			read_callback();
		}
	}

	void dispatcher::update()
	{
		loop->updateDispatcher(this);
	}
}
