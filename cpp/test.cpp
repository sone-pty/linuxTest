#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory>
#include <functional>
#include <deque>

using namespace std;

class te{
public:
	void func()
	{
		cout << "func" << endl;
	}
};

int main(void)
{
	/*
	te t;
	void (te::*ptr)() = &te::func;
	cout << ptr << endl;
	(t.*ptr)();
	*/
	deque<int> que;
	que.push_back(1);
	que.pop_back();
	cout << que.size() << endl;
	que.pop_back();
	cout << que.size() << endl;
	return 0;
}
