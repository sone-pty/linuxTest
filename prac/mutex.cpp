#include "mutex.h"

namespace sone
{
	/*MutexLock*/
	MutexLock::MutexLock()
	{
		pthread_mutex_init(&m_mutex, nullptr);
	}

	MutexLock::~MutexLock()
	{
		pthread_mutex_destroy(&m_mutex);
	}

	void MutexLock::lock()
	{
		pthread_mutex_lock(&m_mutex);
	}

	void MutexLock::unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}

	/*SpinLock*/
	SpinLock::SpinLock()
	{
		pthread_spin_init(&m_slock, 0);
	}

	SpinLock::~SpinLock()
	{
		pthread_spin_destroy(&m_slock);
	}

	void SpinLock::lock()
	{
		pthread_spin_lock(&m_slock);
	}

	void SpinLock::unlock()
	{
		pthread_spin_unlock(&m_slock);
	}
	
	/*CASLock*/
	void CASLock::lock()
	{
		while(flag.test_and_set());
	}

	void CASLock::unlock()
	{
		flag.clear();
	}

	/*Semaphore*/
	Semaphore::Semaphore(u_int32_t count)
	{
		if(sem_init(&m_sem, 0, count) != 0)
			throw std::logic_error("sem_init error");
	}

	Semaphore::~Semaphore()
	{
		sem_destroy(&m_sem);
	}

	void Semaphore::wait()
	{
		while(sem_wait(&m_sem) == -1 && errno == EINTR);
	}

	void Semaphore::notify()
	{
		sem_post(&m_sem);
	}
}
