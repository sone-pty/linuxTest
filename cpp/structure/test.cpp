#include "m_vector.h"
#include <string>
#include <iostream>

using namespace std;
using namespace mst;


int main(void)
{
	m_vector<int> v;
	v.push_back(10);
	v.push_back(6);
	v.push_back(3);
	v.push_back(8);
	v.push_back(1);
	cout << v << endl;
	v.mergeSort(v.begin(), v.end());
	cout << v << endl;
	return 0;
}
