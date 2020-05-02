#include "test.h"
#include "../TimerHeap.h"
#include <signal.h>

void cb()
{
	cout << "callback" << endl;
}

TimerHeap timerheap;

static void sig_alarm(int)
{
	timerheap.tick();
	Timer* tim = new Timer(1);
	tim->setCb(cb);
	timerheap.pushTimer(tim);
	alarm(3);
}

int main(void)
{
	signal(SIGALRM, sig_alarm);
	Timer* tim = new Timer(1);
	tim->setCb(cb);
	timerheap.pushTimer(tim);
	alarm(3);
	while(1);
	return 0;
}
