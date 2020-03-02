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
	void setS(const string&& sl)
	{
		s = sl;
	}
	string getS()
	{
		return s;
	}
private:
	int i;
	string s;
};

void printChar(string s)
{
	for(auto c : s)
		printf("%X ", c);
	printf("\n");
}

enum class c{
	k = 1,
	d = 4
};

ostream& operator<<(ostream& os, c l)
{
	switch(l)
	{
		case c::d:
			os << 4;break;
		case c::k:
			os << 1;break;
	}
	return os;
}

te t;
void func()
{
	string s = "asd";
	t.setS(move(s));
}

int main(void)
{
	/*
	te t;
	void (te::*ptr)() = &te::func;
	cout << ptr << endl;
	(t.*ptr)();
	*/
	func();
	cout << t.getS() << endl;
	return 0;
}
