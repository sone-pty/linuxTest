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
#include <set>
#include <bitset>
#include <queue>
#include <regex>

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

void build(vector<int>& vec, string& p)
{
	int j = 0, t = -1;
	vec[0] = -1;

	while(j < (int)p.length())
	{
		if(t < 0 || p[j] == p[t])
		{
			++t;++j;
			vec[j] = t;
		}
		else
			t = vec[t];
	}
}

int kmp(string s, string p)
{
	vector<int> ne(p.length());
	build(ne, p);

	int i = 0, j = -1;

	while(i < (int)s.length() && j < (int)p.length())
	{
		if(j < 0 || s[i] == p[j])
		{
			++i;++j;
		}
		else
			j = ne[j];
	}
	return i - j;
}

int main(void)
{
	/*
	te t;
	void (te::*ptr)() = &te::func;
	cout << ptr << endl;
	(t.*ptr)();
	return 0;
	*/
	smatch res;
	regex r("[a-z]{3,3}, ([0-9]{1,2}) ([a-z]{3,3}) ([0-9]{4,4}) ([0-9][0-9]:[0-9][0-9]:[0-9][0-9]) GMT", regex::icase);
	string str = "Sat, 7 Jun 2015 16:48:38 GMT";
	if(regex_search(str, res, r))
	{
		cout << res.str(1) << endl;
		cout << res.str(2) << endl;
		cout << res.str(3) << endl;
		cout << res.str(4) << endl;
	}
	string ss = "09";
	int i = atoi(ss.c_str());
	cout << i << endl;
}
