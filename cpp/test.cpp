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
	te() 
	{
		cout << "construct te" << endl;
	}
	~te()
	{
		cout << "delete te" << endl;
	}
private:
	int i;
};

unique_ptr<string> ptr(new string("sdsd"));

unique_ptr<string> func()
{
	return move(ptr);
}

void printChar(string s)
{
	for(auto c : s)
		printf("%X ", c);
	printf("\n");
}

#define ADD(a, b) printf(#a" + "#b" = %d\n", a + b)
int main(void)
{
	/*
	te t;
	void (te::*ptr)() = &te::func;
	cout << ptr << endl;
	(t.*ptr)();
	*/
	string s1 = "你好";
	printChar(s1);
	string s2 = boost::locale::conv::from_utf(s1.c_str(), "gbk");
	printChar(s2);
	return 0;
}
