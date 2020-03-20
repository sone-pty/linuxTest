#include <iostream>
#include <string.h>

using namespace std;

class mstring{
public:
	mstring()
		:buf(nullptr), len(0) {}
	mstring(char* s, int l)
	{
		buf = new char[l + 1];
		std::copy(s, s + l, this->buf);
		len = l;
		buf[len] = '\0';
	}
	~mstring() { delete buf; }
	mstring(mstring&& ms)
	{
		cout << "移动构造" << endl;
		buf = ms.buf;
		ms.buf = nullptr;
		len = ms.len;
	}
	mstring(const mstring& ms)
	{
		cout << "拷贝构造" << endl;
		std::copy(ms.buf, ms.buf + len, buf);
		len = ms.len;
	}
	int length() const { return len; }
	mstring& operator=(const mstring& ms)
	{
		cout << "拷贝赋值" << endl;
		std::copy(ms.buf, ms.buf + len, buf);
		return *this;
	}
	mstring& operator=(mstring&& ms)
	{
		cout << "移动赋值" << endl;
		this->buf = ms.buf;
		this->len = ms.len;
		ms.buf = nullptr;
		return *this;
	}
	char* getBuf() const { return buf; }
private:
	char* buf;
	int len;
};

class user{
public:
	void setms(const mstring& ms)
	{
		this->ms = ms;
	}
	void setms(mstring&& m)
	{
		this->ms = move(m);
	}
	char *getms() const { return ms.getBuf();  }

private:
	mstring ms;
};

int main(void)
{
	char buf[64] = "hello";
	mstring ms(buf, strlen(buf));
	user s;
	s.setms(move(ms));
	if(ms.getBuf() == nullptr)
		cout << "yes" << endl;
	cout << s.getms() << endl;
	return 0;
}
