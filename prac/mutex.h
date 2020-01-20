#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>
#include "nocopyable.h"
#include <semaphore.h>
#include <errno.h>
#include <stdexcept>
#include <memory>
#include <atomic>

namespace sone
{

template <typename T> class ScopedLock;

//互斥锁
class MutexLock : nocopyable{
public:
	typedef ScopedLock<MutexLock> Lock;
	
	MutexLock();
	~MutexLock();
	void lock();
	void unlock();
	pthread_mutex_t* getMutex(){ return &m_mutex; }
private:
	pthread_mutex_t m_mutex;
};

//自旋锁
class SpinLock : nocopyable{
public:
	typedef ScopedLock<SpinLock> Lock;

	SpinLock();
	~SpinLock();
	void lock();
	void unlock();
private:
	pthread_spinlock_t m_slock;
};

//信号量
class Semaphore : nocopyable{
public:
	typedef std::shared_ptr<Semaphore> ptr;

	Semaphore(u_int32_t count = 0);
	~Semaphore();
	void wait();
	void notify();
private:
	sem_t m_sem;
};

//原子锁
class CASLock : nocopyable{
public:
	typedef ScopedLock<CASLock> Lock;

	CASLock() = default;
	~CASLock() = default;
	void lock();
	void unlock();
private:
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

//包装类RAII
template <typename T> class ScopedLock{
public:
	ScopedLock(T&);
	~ScopedLock();
	void lock();
	void unlock();
	bool isLocked() const { return m_locked; }
private:
	//锁状态
	bool m_locked;
	//需要用引用类型，因为锁不可拷贝
	T& m_mutex;
};

/*ScopedLock*/
template <typename T> ScopedLock<T>::ScopedLock(T& m): m_mutex(m)
{
	//不能调用成员函数，需要手动加锁
	m_mutex.lock();
	m_locked = true;
}

template <typename T> ScopedLock<T>::~ScopedLock()
{
	unlock();
}

template <typename T> void ScopedLock<T>::lock()
{
	if(!m_locked)
	{
		m_mutex.lock();
		m_locked = true;
	}
}

template <typename T> void ScopedLock<T>::unlock()
{
	if(m_locked)
	{
		m_mutex.unlock();
		m_locked = false;
	}
}

}

#endif
