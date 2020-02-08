#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory>
#include <functional>
#include <deque>
#include <vector>

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
	void func()
	{
		cout << "func" << endl;
	}
};
vector<int> printMatrix(vector<vector<int> > matrix) {
        vector<int> res;

        while(matrix.size() != 0)
        {
            int rows = matrix.size(), colums = matrix[0].size();
			if(rows == 1)
            {
                for(auto i : matrix[0])
                    res.push_back(i);
                break;
            }
			for(int i = 0;i < colums;++i)
            {
                res.push_back(matrix[0][0]);
                matrix[0].erase(matrix[0].begin());
            }
            for(int i = 1;i < rows;++i)
            {
                res.push_back(matrix[i][colums - 1]);
                matrix[i].erase(matrix[i].begin() + colums - 1);
            }
            for(int i = colums - 2;i >= 0;--i)
            {
                res.push_back(matrix[rows - 1][i]);
                matrix[rows - 1].erase(matrix[rows - 1].begin() + i);
            }
            for(int i = rows - 2;i >= 1;--i)
            {
                res.push_back(matrix[i][0]);
                matrix[i].erase(matrix[i].begin());
            }
			matrix.erase(matrix.begin());
			matrix.erase(matrix.begin() + rows - 1);
        }
        return res;
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
	return 0;
}
