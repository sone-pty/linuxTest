#include "mutex.h"
#include "Thread.h"
#include <iostream>
#include <string>

#define NUMS 5
#define LEFT(i) ((i - 1 + NUMS) % NUMS)
#define RIGHT(i) ((i + 1) % NUMS)

enum STATE
{
	THINKING,
	EATING,
	HUNGRY
};

using namespace sone;
using namespace std;

//临界区的互斥，初始值为1
Semaphore Mutex(1);
//每个哲学家的状态
STATE states[NUMS];
//每个哲学家对应的信号量，初始值为0
Semaphore s[NUMS];
//标准输出的互斥
Semaphore output_mutex(1);

void test(int i)
{
	if(states[i] == HUNGRY && states[LEFT(i)] != EATING && states[RIGHT(i)] != EATING)
	{
		states[i] = EATING;
		s[i].notify();
	}
}

//拿起叉子
void take_fork(int i)
{
	Mutex.wait();
	states[i] = HUNGRY;
	//尝试取得叉子
	test(i);
	Mutex.notify();
	//如果拿不到两个叉子就阻塞
	s[i].wait();
}

//放下叉子
void put_fork(int i)
{
	Mutex.wait();
	states[i] = THINKING;
	//尝试让左右两边的哲学家拿起叉子
	test(LEFT(i));
	test(RIGHT(i));
	Mutex.notify();
}

void think(int i)
{
	output_mutex.wait();
	cout << "哲学家" << i << "正在思考" << endl;
	output_mutex.notify();
	sleep(1);
}

//线程函数，i表示哲学家的编号；从0开始
void philosopher(int i)
{
	while(1)
	{
		think(i);
		take_fork(i);
		output_mutex.wait();
		cout << "哲学家" << i << "正在吃饭" << endl;
		output_mutex.notify();
		sleep(1);
		put_fork(i);
		output_mutex.wait();
		cout << "哲学家" << i << "吃完饭了" << endl;
		output_mutex.notify();
		sleep(1);
	}
}

int main(void)
{
	for(int i = 0;i < NUMS;++i)
		new Thread(to_string(i), bind(philosopher, i));
	while(1);
	return 0;
}
