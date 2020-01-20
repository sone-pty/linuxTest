#include "../FixedThreadPool.h"
#include "test.h"

using namespace std;
using namespace sone;

MutexLock lock;

void run()
{
	MutexLock::Lock l(lock);
	cout << "current threadid:" << syscall(SYS_gettid) << endl;
}

void print(const string& s)
{
	MutexLock::Lock l(lock);
	cout << s << endl;
}

int main(void)
{
	FixedThreadPool tp;
	tp.start(5);
	tp.addTask(run);
	tp.addTask(run);
	tp.addTask(bind(print, "123"));

	tp.stop();
	return 0;
}
