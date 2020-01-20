#ifndef TIMEWHEEL_H_
#define TIMEWHEEL_H_

#include <time.h>
#include <netinet/in.h>
#include <functional>
#include <memory>

struct tw_timer;

//用户数据类
struct user_data{
	typedef std::shared_ptr<user_data> ptr;
	
	const static int BUFFSIZE = 64;
	int fd;
	char buf[BUFFSIZE];
	sockaddr_in addr;
	std::shared_ptr<tw_timer> timer;	
};

//定时器类
struct tw_timer{
	//记录该定时器需要转多少圈
	int rotation;
	//记录该定时器在轮中的哪个槽
	int slot;
	//回调
	std::function<void(user_data*)> func;
	//客户数据
	std::shared_ptr<user_data> c_data;
	tw_timer* next;
	tw_timer* prev;
};

//时间轮类
class timeWheel{
public:
	timeWheel():curslot(0)
	{
		for(int i = 0;i < 60;++i)
		{
			slots[i] = nullptr;
		}
	}
	~timeWheel()
	{
		for(int i = 0;i < 60;++i)
		{
			auto tmp = slots[i];
			while(tmp)
			{
				slots[i] = tmp->next;
				delete tmp;
				tmp = slots[i];
			}
		}
	}
private:
	const static int SI = 1;
	tw_timer* slots[60];
	int curslot;
};

#endif
