#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <fstream>
#include <functional>
#include "util.h"
using namespace std;


class strblob
{
public:
	typedef vector<string>::size_type size_type;
	strblob():s_ptr(make_shared<vector<string>>()){}
	
	strblob(initializer_list<string> il):s_ptr(make_shared<vector<string>>(il)){}
	
	bool empty() const
	{
		return s_ptr->empty();
	}

	size_type size() const
	{
		return s_ptr->size();
	}

	void push_back(const string& s)
	{
		s_ptr->push_back(s);
	}

	void pop_back()
	{
		s_ptr->pop_back();
	}

	const string& front() const
	{
		if(size() == 0)
			throw out_of_range("当前容器为空！");
		return (*s_ptr)[0];
	}

	string& front()
	{
		if(size() == 0)
			throw out_of_range("当前容器为空！");
		return (*s_ptr)[0];
	}

	const string& back() const
	{
		if(size() == 0)
			throw out_of_range("当前容器为空！");
		return (*s_ptr)[size() - 1];
	}

	string& back()
	{
		if(size() == 0)
			throw out_of_range("当前容器为空！");
		return (*s_ptr)[size() - 1];
	}

	void printall() const
	{
		for(auto s : *s_ptr)
			cout << s << " ";
		cout << endl;
	}
private:
	shared_ptr<vector<string>> s_ptr;
};

class parent{
public:
	parent()
	{
		cout << "调用了parent类的构造函数" << endl;
	}
	~parent()
	{
		cout << "调用了parent类的析构函数" << endl;
	}

	parent(const parent& p):s(p.s)
	{

	}

	virtual void dynamic_func(string& tip)//加上final表示该虚函数不可被覆盖
	{
		cout << "parent's tip = " << tip << endl;
	}
private:
	string s;
};

class B{
public:
	B()
	{
		cout << "调用了B的默认构造函数" << endl;
	}
	B(int)
	{
		cout << "调用了B的构造函数" << endl;
	}
	B(const B&)
	{
		cout << "调用了B的拷贝构造函数" << endl;
	}
	~B()
	{
		cout << "调用了B的析构函数" << endl;
	}
};

class A : public parent{
public:
	A() = default;
	A(int v):val(v)
	{
		b = B(1);
	}
	A(const A& a,int size = 10):parent(a),val(a.val),b(a.b),vec(size)
	{
		cout << "调用了A的拷贝构造" << endl;
		cout << "vec的size = " << vec.size() << endl;
	}
	~A()
	{
		cout << "调用了A的析构函数" << endl;
	}

	void dynamic_func(string& tip) override //加上override表示该函数必须重写（覆盖）基类的虚函数而不能隐藏
	{
		cout << "A's tip = " << tip << endl;
	}

	void wrapper_prifunc()
	{
		pri_func();
	}
private:
	int val;
	B b;
	vector<int> vec;
	virtual void pri_func()
	{
		cout << "A::pri_func" << endl;
	}
};

class derived : public A{
public:
	derived() = default;
	void func_derived()
	{

	}
	void pri_func()
	{
		cout << "derived::pri_func" << endl;
	}
};

void f(A)
{
	cout << "f()" << endl;
	cout << "局部A对象即将析构" << endl;
}

template <typename T> T selectCalc(string op, T v1, T v2, map<string, function<T(T,T)>>& m)
{
	return m[op](v1, v2);
}

//为模板类型定义别名，typedef只能为实例化模板类型起别名，twin<int,string>相当于 pair<pair<int,int>,pair<string,string>>
template <typename T, typename V> using twin = pair<pair<T,T>,pair<V,V>>;

template <typename T> class temclass{
public:
	temclass(const T v):val(v)
	{
		cout << "构造temclass" << endl;
	}
	void print()
	{
		cout << val << endl;
	}
private:
	T val;
};

//模板特化
template <> void temclass<int>::print()
{
	cout << val + 1<< endl;
}

int main(void)
{
	/*
	 * allocator 示例
	allocator<string> alloc = allocator<string>();
	string* p = alloc.allocate(10);
	string* q = p;
	string* index = p;
	for(int i = 0;i < 10;i++)
		alloc.construct(q++,"s" + to_string(i));
	
	while(index != q)
		cout << *index++ << endl;

	while(q != p)
		alloc.destroy(--q);

	alloc.deallocate(p,10);
	*/
	/*
	 * 如果传递临时对象，在传递参数的时候不会调用拷贝构造函数，比如f(A(2))
	A a(1);
	f(a);
	cout << "a析构之前" << endl;
	*/
	/*
	map<string, function<double(double,double)>> m;
	m.insert(make_pair("+",[](double d1,double d2)->double{ return d1 + d2; }));
	m.insert(make_pair("-",[](double d1,double d2)->double{ return d1 - d2; }));
	m.insert(make_pair("*",[](double d1,double d2)->double{ return d1 * d2; }));
	m.insert(make_pair("/",[](double d1,double d2)->double{ return d1 / d2; }));
	double d1 = 6.8, d2 = 3.5;
	cout << selectCalc("+", d1, d2, m) << endl;
	*/
	/*
	int val = 2;
	int& a = val;
	cout << "&val = " << &val << endl;
	cout << "&a = " << &a << endl;
	typename remove_reference<int&>::type p = val;
	cout << "&p = " << &p << endl;
	*/
	return 0;
}
