#ifndef VECTOR_H_
#define VECTOR_H_

#include <unistd.h>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace mst
{	
	class Fib { //Fibonacci数列类
	private:
		int f, g; //f = fib(k - 1), g = fib(k)。均为int型，很快就会数值溢出
	public:
		Fib ( int n ) //初始化为不小于n的最小Fibonacci项
		{ 
			f = 1; g = 0; 
			while ( g < n ) 
				next(); 
		}
		int get()  { return g; } //获取当前Fibonacci项，O(1)时间
		int next() { g += f; f = g - f; return g; } //转至下一Fibonacci项，O(1)时间
		int prev() { f = g - f; g -= f; return g; } //转至上一Fibonacci项，O(1)时间
	};

	//前置声明
	template <typename> class m_vector;
	template <typename T> std::ostream& operator<<(std::ostream& os, const m_vector<T>& v);
	
	//规定所有带有迭代器区间的函数都是左开右闭的形式，比如[lo,hi)不包含hi
	template <typename T> class m_vector{
	public:
		typedef T* m_iterator;
		typedef const T* m_const_iterator;
		
		friend std::ostream& operator<< <T>(std::ostream& os, const m_vector<T>& v);

		//构造函数
		explicit m_vector(size_t size)
		{
			m_size = size;
			m_capcity = 2 * m_size;
			vec = new T[m_capcity];
			std::fill(vec, vec + m_size, 0);
		}

		m_vector(size_t size, T val)
		{
			this->m_size = size;
			this->m_capcity = m_size * 2;
			vec = new T[m_capcity];
			std::fill(vec, vec + m_size, val);
		}

		m_vector():m_capcity(DEFAULT_SZIE)
		{
			vec = new T[DEFAULT_SZIE];
		}

		m_vector(const m_vector& v):m_capcity(v.m_capcity),m_size(v.m_size)
		{
			if(v == *this)
				throw std::logic_error("不能拷贝自己！");
			vec = new T[m_capcity];
			for(int i = 0;i < m_size;++i)
				vec[i] = v[i];
		}

		m_vector(m_vector&& v):m_capcity(v.m_capcity),m_size(v.m_size),vec(v.vec)
		{
			v.vec = nullptr;
		}
		
		//运算符重载
		T& operator[](size_t pos) const
		{
			if(pos >= m_size)
				throw std::out_of_range("下标越界！");
			else
				return vec[pos];
		}

		m_vector& operator=(const m_vector& v)
		{
			delete[] vec;
			m_capcity = v.m_capcity;
			m_size = v.m_size;
			vec = new T[m_capcity];
			
			for(int i = 0;i < m_size;++i)
				vec[i] = v[i];
			return *this;
		}

		m_vector& operator=(m_vector&& v)
		{
			delete[] vec;
			vec = v.vec;
			m_capcity = v.m_capcity;
			m_size = v.m_size;
			v.vec = nullptr;
			return *this;
		}
		
		//析构
		~m_vector()
		{
			delete[] vec;
		}

		//成员函数
		size_t size() const
		{
			return m_size;
		}

		bool empty() const
		{
			return m_size == 0;
		}

		m_iterator begin()
		{
			return vec;
		}

		m_const_iterator begin() const
		{
			return vec;
		}

		m_const_iterator end() const
		{
			return vec + m_size;
		}

		m_iterator end()
		{
			return vec + m_size;
		}

		size_t capacity() const
		{
			return m_capcity;
		}
		
		m_iterator find(const T& e, m_iterator start, m_iterator end);
		m_iterator find(const T& e) const;

		//有序向量的查找
		m_iterator search(const T& e, m_iterator lo, m_iterator hi);
		m_iterator search(const T& e){ return search(e, begin(), end()); }
		m_iterator fibsearch(const T& e, m_iterator lo, m_iterator hi);
		
		T remove(m_iterator pos);
		void insert(m_iterator pos, T val);
		void push_back(T val);
		void pop_back();
		T& tail() const;
		void erase(m_iterator l, m_iterator r);
		//无序向量的去重
		int deduplicate();
		//有序向量的去重
		int unique();

		//排序算法
		void bubbleSort(m_iterator lo, m_iterator hi){ while(lo < (hi = bubble(lo, hi))); }
		void mergeSort(m_iterator lo, m_iterator hi);
	private:
		//容量
		size_t m_capcity;
		//当前向量中的元素数量
		size_t m_size = 0;
		//默认大小的容量
		const static size_t DEFAULT_SZIE = 16;
		//动态数组
		T* vec;
	private:
		//容量扩充
		void reserve(size_t newcapcity);
		//冒泡排序辅助算法
		m_iterator bubble(m_iterator lo, m_iterator hi);
		//归并例程
		void merge(m_iterator lo, m_iterator mid, m_iterator hi);
	};
	
	template <typename T> void m_vector<T>::mergeSort(m_iterator lo, m_iterator hi)
	{
		if(hi - lo < 2)
			return;
		m_iterator mid = lo + (hi - lo) / 2;
		mergeSort(lo, mid);
		mergeSort(mid, hi);
		merge(lo, mid, hi);
	}
	
	template <typename T> void m_vector<T>::merge(m_iterator lo, m_iterator mid, m_iterator hi)
	{
		T* temp = new T[hi - lo];
		m_iterator p1 = lo, p2 = mid;
		int index = 0;

		while(p1 != mid && p2 != hi)
		{
			if(*p1 < *p2)
				temp[index++] = *p1++;
			else
				temp[index++] = *p2++;
		}

		if(p1 == mid && p2 != hi)
		{
			while(p2 != hi)
				temp[index++] = *p2++;
		}
		else if(p1 != mid && p2 == hi)
		{
			while(p1 != mid)
				temp[index++] = *p1++;
		}
		
		m_iterator pr = temp;
		for(auto p = lo;p < hi;p++)
			*p = *pr++;

		delete[] temp;
	}

	template <typename T> typename m_vector<T>::m_iterator m_vector<T>::bubble(m_iterator lo, m_iterator hi)
	{
		m_iterator p = lo, last = lo;
		
		while(++p < hi)
		{
			if(*p < *(p - 1))
			{
				last = p;
				std::swap(*p, *(p - 1));
			}
		}

		return last;
	}

	template <typename T> typename m_vector<T>::m_iterator m_vector<T>::fibsearch(const T& e, m_iterator lo, m_iterator hi)
	{
		Fib fib(hi - lo);
		m_iterator mid, end = hi;

		while(lo < hi)
		{
			while(hi - lo < fib.get())
				fib.prev();

			mid = lo + fib.get() - 1;
			if(*mid < e)
				lo = mid + 1;
			else if(e < *mid)
				hi = mid;
			else
				return mid;
		}

		return end;
	}

	template <typename T> typename m_vector<T>::m_iterator m_vector<T>::search(const T& e, m_iterator lo, m_iterator hi)
	{
		ptrdiff_t len = hi - lo;
		m_iterator start = lo, end = hi, mid = lo + (len / 2);
		
		while(start < end)
		{
			mid = start + (end - start) / 2;
			if(*mid < e)
				start = mid + 1;
			else if(e < *mid)
				end = mid;
			else
				return mid;
		}

		return hi;
	}

	template <typename T> typename m_vector<T>::m_iterator m_vector<T>::find(const T& e, m_iterator start, m_iterator end)
	{
		for(auto p = start;p != end;++p)
		{
			if(*p == e)
				return p;
		}
		return end;
	}

	template <typename T> int m_vector<T>::deduplicate()
	{
		auto beg = begin();
		int i = -1;
		int old_size = m_size;

		while(++i < m_size)
		{
			int j = i + 1;
			while(j < m_size)
			{
				if(vec[j] == vec[i])
					this->remove(beg + j);
				else
					j++;
			}
		}

		return old_size - m_size;
	}

	template <typename T> int m_vector<T>::unique()
	{
		int i = 0,j = 0;

		while(++j < m_size)
		{
			if(vec[i] != vec[j])
				vec[++i] = vec[j];
		}
		m_size = i + 1;
	}

	template <typename T> void m_vector<T>::erase(m_iterator l, m_iterator r)
	{
		if(l >= r)
			return;
		while(r != end())
		{
			*l++ = std::move(*r);
			r++;
		}
		m_size -= r - l;
	}

	template <typename T> typename m_vector<T>::m_iterator m_vector<T>::find(const T& e) const
	{
		int i;
		for(i = 0;i < size();++i)
		{
			if(vec[i] == e)
				break;
		}

		return vec + i;
	}

	template <typename T> T m_vector<T>::remove(m_iterator pos)
	{
		T res = *pos;
		for(auto p = pos;p != end() - 1;++p)
			*p = std::move(*(p + 1));
		m_size--;
		return res;
	}

	template <typename T> void m_vector<T>::insert(m_iterator pos, T val)
	{
		if(m_size + 1 > m_capcity)
		{
			size_t diff = pos - vec;
			reserve(m_size * 2);
			pos = vec + diff;
		}
		m_size++;
		for(auto p = end() - 1;p != pos;--p)
			*p = *(p - 1);
		*pos = val;
	}

	template <typename T> void m_vector<T>::reserve(size_t newcapcity)
	{
		T* temp = new T[newcapcity];
		for(int i = 0;i < m_size;++i)
			*(temp + i) = std::move(vec[i]);
		m_capcity = newcapcity;
		delete vec;
		vec = temp;
	}

	template <typename T> void m_vector<T>::push_back(T val)
	{
		if(m_size + 1 > m_capcity)
			reserve(m_size * 2);
		vec[m_size] = val;
		m_size++;
	}

	template <typename T> void m_vector<T>::pop_back()
	{
		vec[m_size - 1] = 0;
		m_size--;
	}

	template <typename T> T& m_vector<T>::tail() const
	{
		return vec[m_size - 1];
	}

	template <typename T> std::ostream& operator<<(std::ostream& os, const m_vector<T>& v)
	{
		for(int i = 0;i < v.size();++i)
			os << v[i] << " ";
		os << std::endl;
		return os;
	}
}

#endif
