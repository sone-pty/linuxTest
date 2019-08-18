#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <std.h>

int Max(int a,int b);

int lcsWithRecursion1(const char* s1,int m,const char* s2,int n);

int lcsWithRecursion2(const char* s1,int m,const char* s2,int n);

template <typename T> void bubbleSort(T* vec,int lo,int hi)
{
    int low = lo,high = hi;
    int last;
    int i = low,j = low + 1;

    while(low < high)
    {
        last = low;
        i = low,j = low + 1;
        while(j <= high)
        {
            if(vec[i] > vec[j])
            {
                int temp = vec[j];
                vec[j] = vec[i];
                vec[i] = temp;
                last = j;
            }
            i++;
            j++;
        }
        high = last;
    }
}

template <typename T> void merge(T* vec,int low,int mid,int high)
{
    T* v1 = new T[mid - low + 1],*v2 = new T[high - mid];

    for(int i = 0;i < mid - low + 1;i++)
        v1[i] = vec[i + low];

    for(int i = 0;i < high - mid;i++)
        v2[i] = vec[i + mid + 1];

    int i = 0,j = 0,index = low;
    while(i < mid - low + 1 || j < high - mid)
    {
        if(j < high - mid && (i >= mid - low + 1 || v1[i] > v2[j]))
            vec[index++] = v2[j++];
        if(i < mid - low + 1 && (j >= high - mid || v2[j] > v1[i]))
            vec[index++] = v1[i++];
    }

    delete[] v1;
    delete[] v2;
}

template <typename T> void mergeSort(T* vec,int low,int high)
{
    if(low == high)
        return;

    int mid = (low + high) >> 1;

    mergeSort(vec,low,mid);
    mergeSort(vec,mid + 1,high);
    merge(vec,low,mid,high);
}

template <typename T> class te
{
public:
    void kill(T x);
};



#endif // ALGORITHMS_H
