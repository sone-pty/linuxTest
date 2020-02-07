#include "eventloop.h"
#include <assert.h>
#include "dispatcher.h"
#include "epoller.h"
#include <sys/eventfd.h>

namespace sone
{
	const static int EPOLL_TIMEOUT = 10000;

	eventloop::eventloop()
		:_epoller(new epoller(this)), quit(false), threadId(util::getTid())
	{
		wakeupfd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
		if(wakeupfd < 0)
		{
			SONE_LOG_ERR() << "eventloop::eventloop()-eventfd()";
			abort();
		}
		wakeup_dispatcher.reset(new dispatcher(wakeupfd, this));
		wakeup_dispatcher->enableReading();
		wakeup_dispatcher->setReadCallback(std::bind(&eventloop::process_wakeup_read, this));
	}

	eventloop::~eventloop()
	{
		wakeup_dispatcher->disableReading();
		_epoller->delDispatcher(wakeup_dispatcher.get());
		::close(wakeupfd);
	}
	
	void eventloop::process_wakeup_read()
	{
		int64_t data;
		::read(wakeupfd, &data, sizeof(data));
	}

	void eventloop::updateDispatcher(dispatcher* ed)
	{
		assert(ed->getEventloop() == this);
		_epoller->setDispatcher(ed);
	}

	void eventloop::startloop()
	{
		while(!quit)
		{
			active_dispatchers.clear();
			ret_time = _epoller->epoll(EPOLL_TIMEOUT, &active_dispatchers);
			for(auto d : active_dispatchers)
				d->process();
			//process Non-IO functors
			std::vector<functor> temp;
			{
				MutexLock::Lock l(_mutex);
				temp.swap(non_io_functors);
			}
			for(const functor& func : temp)
				func();
		}
	}

	void eventloop::runInLoop(std::function<void()> f)
	{
		bool flag = false;
		if(util::getTid() == threadId)
		{
			flag = true;
			f();
		}
		else
		{
			{
				MutexLock::Lock l(_mutex);
				non_io_functors.push_back(std::move(f));
			}
			//如果不在IO线程，需要唤醒以打破可能的阻塞
			if(!flag)
			{
				int64_t data = 1;
				::write(wakeupfd, &data, sizeof(data));
			}
		}
	}

	void eventloop::exit()
	{
		quit = true;
		if(util::getTid() != threadId)
		{
			int64_t data = 1;
			::write(wakeupfd, &data, sizeof(data));
		}
	}

	void eventloop::removeDispatcher(dispatcher* ed)
	{
		if(ed)
			_epoller->delDispatcher(ed);
	}
}
