#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory>
#include <functional>
#include <deque>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/locale.hpp>

using namespace std;

struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};

class te{
public:
	void func() const
	{
		cout << "func" << endl;
	}
private:
	int i;
};

#define ADD(a, b) printf(#a" + "#b" = %d\n", a + b)
int main(void)
{
	/*
	te t;
	void (te::*ptr)() = &te::func;
	cout << ptr << endl;
	(t.*ptr)();
	*/
	string s = "我";
	cout << s[0] << s[1] << s[2] << endl;
	return 0;
}
