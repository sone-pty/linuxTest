#ifndef CONDITION_H_
#define CONDITION_H_

#include <pthread.h>
#include "nocopyable.h"
#include "mutex.h"

namespace sone
{

class Condition : nocopyable{
public:
	explicit Condition(MutexLock& m): m_mutex(m){ pthread_cond_init(&m_condition, nullptr); }
	~Condition(){ pthread_cond_destroy(&m_condition); }
	void wait(){ pthread_cond_wait(&m_condition, m_mutex.getMutex()); }
	void notify(){ pthread_cond_signal(&m_condition); }
	void notifyAll(){ pthread_cond_broadcast(&m_condition); }
private:
	MutexLock& m_mutex;
	pthread_cond_t m_condition;
};

}

#endif
