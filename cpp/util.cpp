#include "util.h"

using namespace std;

bool m_utils::isPrime(unsigned int v)
{
	if(v == 1)
		return true;
	if(v % 2 == 0)
		return false;

	for(int i = 3;i <= sqrt(static_cast<double>(v));++i)
	{
		if(v % i == 0)
			return false;
	}
	
	return true;
}

unsigned int m_utils::nextPrime(unsigned int n)
{
	unsigned int i = n;
	while(!isPrime(++i));
	return i;
}
