#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "std.h"

typedef struct NODE
{
    int value;
    struct NODE* next;
    NODE(int v = 0,NODE* pn = 0)
    {
        value = v;
        next = pn;
    }
}node,*pnode;

class linkedList
{
    friend std::ostream& operator<<(std::ostream& os,const linkedList& list);
    //friend bool operator==(const linkedList& l1,const linkedList& l2);
public:
    linkedList();
    linkedList(std::vector<int>& vec);
    virtual ~linkedList();
    bool isEmpty();
    pnode find(int x);
    bool Delete(int x);
    bool insert(int x,pnode pos);
    bool push(int x);
    int pop();
    int dequeue();
private:
    pnode header;
};

/*double linkedList*/
typedef struct DBNODE
{
    int value;
    DBNODE* next;
    DBNODE* prev;
    bool flag;
    DBNODE(int v = 0,DBNODE* next = 0,DBNODE* prev = 0,bool flag = false)
    {
        this->value = v;
        this->next = next;
        this->prev = prev;
        this->flag = flag;
    }
}dbnode,*pdbnode;

class dbLinkedList
{
    friend std::ostream& operator<<(std::ostream& os,const dbLinkedList& list);
public:
    dbLinkedList();
    virtual ~dbLinkedList();
    bool push(int x);
    bool Delete(int x);
    int getHeader();
private:
    pdbnode head;
    pdbnode tail;
};
#endif // LINKEDLIST_H
