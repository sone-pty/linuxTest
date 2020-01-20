#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <pthread.h>
#include "nocopyable.h"

namespace sone
{

template <typename T> class Singleton : nocopyable{
public:
	Singleton() = delete;
	~Singleton() = delete;

	static T* getInstance();
private:
	static pthread_once_t m_once;
	static T* m_value;
	static void init() { m_value = new T; }
};

template <typename T> T* Singleton<T>::m_value = nullptr;
template <typename T> pthread_once_t Singleton<T>::m_once = PTHREAD_ONCE_INIT;

template <typename T> T* Singleton<T>::getInstance()
{
	pthread_once(&m_once, Singleton::init);
	return m_value;
}

}

#endif
