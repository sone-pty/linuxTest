#include "linkedlist.h"

using namespace std;

/*linkedList*/
linkedList::linkedList()
{
    header = new node();
}

linkedList::linkedList(std::vector<int>& vec)
{
    header = new node();
    for(int i = 0; i < vec.size(); i++)
        this->push(vec[i]);
}

linkedList::~linkedList()
{
    pnode temp;
    pnode p = header->next;

    delete header;
    header = 0;

    while(p != 0)
    {
        temp = p->next;
        delete p;
        p = temp;
    }
}

bool linkedList::isEmpty()
{
    if(header == 0 || header->next == 0)
        return true;
    else
        return false;
}

bool linkedList::insert(int x, pnode pos)
{
    if(pos == NULL)
        return false;

    auto posnext = pos->next;
    pos->next = new node(x);
    pos->next->next = posnext;

    return true;
}

bool linkedList::push(int x)
{
    auto p = header->next;

    if(p == NULL)
    {
        header->next = new node(x);
        return true;
    }

    pnode prev = NULL;

    while(p != NULL)
    {
        prev = p;
        p = p->next;
    }

    prev->next = new node(x);
    return true;
}

pnode linkedList::find(int x)
{
    auto p = header->next;

    while(p != NULL)
    {
        if(p->value == x)
            return p;
        else
            p = p->next;
    }

    return NULL;
}

bool linkedList::Delete(int x)
{
    auto p = header->next;
    pnode prev = header;

    if(p == NULL)
        return false;

    while(p != NULL)
    {
        if(p->value == x)
        {
            prev->next = p->next;
            delete p;
            return true;
        }
        else
        {
            p = p->next;
            prev = prev->next;
        }
    }

    return false;
}

ostream& operator<<(ostream& os,const linkedList& list)
{
    auto p = list.header->next;

    while(p != NULL)
    {
        os << p->value << " ";
        p = p->next;
    }

    return os;
}

int linkedList::pop()
{
    auto p = header->next;
    pnode prev;
    int ret;

    while(true)
    {
        prev = p;
        p = p->next;

        if(p->next == NULL)
        {
            ret = p->value;
            prev->next = NULL;
            delete p;
            return ret;
        }
    }
}

int linkedList::dequeue()
{
    auto p = header->next;
    if(p == NULL)
        return NULL;
    auto ne = p->next;
    int ret = p->value;
    delete p;
    header->next = ne;

    return ret;
}

/*dblinkedList*/
dbLinkedList::dbLinkedList()
{
    this->head = new dbnode();
    head->next = head;
    head->prev = head;
    this->tail = this->head;
}

dbLinkedList::~dbLinkedList()
{
    auto p = this->head;

    while(p != tail)
    {
        auto temp = p->next;
        delete p;
        p = temp;
    }

    delete tail;
    head = tail = 0;
}

bool dbLinkedList::push(int x)
{
    if(head == tail && head->flag == false)
    {
        head->value = x;
        head->flag = true;
        return true;
    }

    tail->next = new dbnode(x,0,0,true);
    tail->next->prev = tail;
    tail = tail->next;
    tail->next = head;
    head->prev = tail;

    return true;
}

bool dbLinkedList::Delete(int x)
{
    if(head == tail && head->flag == false)
        return false;
    else if(head == tail && head->flag == true && x == head->value)
    {
        delete head;
        head = tail = 0;
        return true;
    }

    auto p = head;
    while(true)
    {
        if(p->value == x)
        {
            if(p == head)
            {
                auto ne = p->next;
                delete p;
                ne->prev = tail;
                tail->next = ne;
                head = ne;
            }
            else if(p == tail)
            {
                auto pr = p->prev;
                delete p;
                pr->next = head;
                head->prev = pr;
                tail = pr;
            }
            else
            {
                auto pr = p->prev;
                auto ne = p->next;
                delete p;
                pr->next = ne;
                ne->prev = pr;
            }

            return true;
        }
        if((p = p->next) == head)
            break;
    }

    return false;
}

int dbLinkedList::getHeader()
{
    return head->value;
}
