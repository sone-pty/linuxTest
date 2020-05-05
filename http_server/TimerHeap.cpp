#include "TimerHeap.h"

namespace sone
{
	TimerHeap::TimerHeap(size_t capcity)
		:_vec(capcity), _nums(0)
	{

	}

	TimerHeap::~TimerHeap()
	{
		MutexLock::Lock l(_lock);
		for(size_t i = 0;i < _nums;++i)
			delete _vec[i];
	}

	void TimerHeap::pushTimer(Timer* timer)
	{
		MutexLock::Lock l(_lock);
		if(!timer)
			return;
		if(++_nums <= _vec.capacity())
			_vec[_nums - 1] = timer;
		//上滤
		int h = _nums - 1, p;
		while(h > 0)
		{
			p = (h - 1) >> 1;
			if(_vec[p]->getExpire() > _vec[h]->getExpire())
				std::swap(_vec[p], _vec[h]);
			else
				break;
			h = p;
		}
		timer->setIndex(h);	
	}

	void TimerHeap::popTimer()
	{
		MutexLock::Lock l(_lock);
		
		if(_nums == 0)
			return;

		std::swap(_vec[0], _vec[_nums - 1]);
		//delete _vec[_nums - 1];
		_vec[_nums - 1]->setIndex(-2);
		_vec[_nums - 1] = nullptr;
		--_nums;
		//下滤
		down(0);
	}

	void TimerHeap::down(size_t index)
	{
		size_t child;
		while(index < _nums)
		{
			if(2 * index + 2 < _nums)
				child = (_vec[2 * index + 1]->getExpire() < _vec[2 * index + 2]->getExpire()) ? 2 * index + 1 : 2 * index + 2;
			else if(2 * index + 1 < _nums)
				child = 2 * index + 1;
			else
				break;
			if (_vec[index]->getExpire() > _vec[child]->getExpire())
			{
				std::swap(_vec[index], _vec[child]);
				index = child;
			}
			else
				break;
		}
	}

	Timer* TimerHeap::topTimer()
	{
		MutexLock::Lock l(_lock);
		if(_nums != 0)
			return _vec[0];
		else
			return nullptr;
	}

	void TimerHeap::tick()
	{
		Timer* timer;
		time_t cur = time(nullptr);
		std::function<void()> cb;

		while(!empty())
		{
			timer = topTimer();
			if(!timer || timer->getExpire() > cur)
				break;
			cb = timer->getCb();
			if(cb)
				cb();
			popTimer();
		}
	}

	bool TimerHeap::empty()
	{
		MutexLock::Lock l(_lock);
		return (_nums == 0);
	}
}
