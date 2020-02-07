#include "../std.h"

class LRUCache {
public:
    struct node
    {
        int key;
        int value;
        struct node* next;
        struct node* prev;
        node():key(-1),value(-1),next(NULL),prev(NULL){}
    };
    
    LRUCache(int capacity) {
        this->capacity = capacity;
        cursize = 0;
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        auto iter = m.find(key);
        if(iter == m.end())
            return -1;
        auto inode = iter->second;
        auto temp = head->next;
        if(temp != inode)
        {
            auto tprev = inode->prev, tnext = inode->next;
            head->next = inode;
            inode->prev = head;
            inode->next = temp;
            temp->prev = inode;
            tprev->next = tnext;
            tnext->prev = tprev;
        }
        
        return inode->value;
    }
    
    void put(int key, int value) {
        auto n = new node();
        n->value = value;
        n->key = key;
        auto iter = m.find(key);
        
        if(iter == m.end())
        {
            //缓存未满
            if(cursize < capacity)
            {
                auto temp = head->next;
                head->next = n;
                n->next = temp;
                temp->prev = n;
                n->prev = head;
                m.insert(make_pair(key,n));
                cursize++;
            }
            //缓存满的话，删除最后一个节点，插入新节点在头部
            else
            {
                auto end = tail->prev;
                end->prev->next = tail;
                tail->prev = end->prev;
                auto temp = head->next;
                head->next = n;
                n->prev = head;
                n->next = temp;
                temp->prev = n;
                m.erase(m.find(end->key));
                m.insert(make_pair(key,n));
            }
        }
        else
        {
            iter->second->value = value;
            auto inode = iter->second;
            auto temp = head->next;
            if(temp != inode)
            {
                auto tprev = inode->prev, tnext = inode->next;
                head->next = inode;
                inode->prev = head;
                inode->next = temp;
                temp->prev = inode;
                tprev->next = tnext;
                tnext->prev = tprev;
            }
        }
    }
private:
    node* head;
    node* tail;
    int capacity;
    int cursize;
    unordered_map<int,node*> m;
};
