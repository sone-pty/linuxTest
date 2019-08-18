#ifndef TREE_H
#define TREE_H

#include "std.h"

template <typename T> struct treenode
{
    T value;
    treenode* left;
    treenode* right;
    treenode(T value,treenode<T>* left = 0,treenode<T>* right = 0)
    {
        this->value = value;
        this->left = left;
        this->right = right;
    }
};

template <typename T> class SearchTree
{
    template <typename X> friend std::ostream& operator<<(std::ostream& os,const SearchTree<X>& st);
public:
    SearchTree(const std::vector<T>& vec);
    SearchTree();
    virtual ~SearchTree();
    bool insert(T v);
    bool Delete(T v);
    T findMin();
private:
    treenode<T>* root;
    bool insert(T v,treenode<T>* pnode,treenode<T>* parent,bool isleft);
    treenode<T>* find(T v,treenode<T>* root);
    treenode<T>* findMin(treenode<T>* root);
    void Delete(treenode<T>* root);
    treenode<T>* Delete(T v,treenode<T>* root);
};

template <typename T> SearchTree<T>::SearchTree()
{
    root = NULL;
}

template <typename T> SearchTree<T>::SearchTree(const std::vector<T>& vec)
{
    root = NULL;
    int size = vec.size();

    insert(vec[size / 2]);
    int i = size / 2 + 1,j = size / 2 - 1;
    for(; i < vec.size(); i++,j--)
    {
        insert(vec[i]);
        insert(vec[j]);
    }

    if(j == 0)
        insert(vec[j]);
}

template <typename T> SearchTree<T>::~SearchTree()
{
    Delete(root);
}

template <typename T> void SearchTree<T>::Delete(treenode<T>* root)
{
    auto left = root->left;
    auto right = root->right;

    delete root;
    if(left != NULL)
        Delete(left);
    else
        return;
    if(right != NULL)
        Delete(right);
    else
        return;
}

template <typename T> treenode<T>* SearchTree<T>::findMin(treenode<T>* root)
{
    treenode<T>* res = NULL;
    treenode<T>* p = root;
    while(p != NULL)
    {
        if(p->left == NULL)
            res = p;
        p = p->left;
    }

    return res;
}

template <typename T> T SearchTree<T>::findMin()
{
    return findMin(root)->value;
}

template <typename T> bool SearchTree<T>::insert(T v)
{
    if(root == NULL)
    {
        root = new treenode<T>(v);
        return true;
    }

    if(v < root->value)
        return insert(v,root->left,root,true);
    else if(v > root->value)
        return insert(v,root->right,root,false);
    else
        return false;
}

template <typename T> bool SearchTree<T>::insert(T v,treenode<T>* pnode,treenode<T>* parent,bool isleft)
{
    if(pnode == NULL)
    {
        pnode = new treenode<T>(v);
        if(isleft)
            parent->left = pnode;
        else
            parent->right = pnode;
        return true;
    }

    if(v < pnode->value)
        return insert(v,pnode->left,pnode,true);
    else if(v > pnode->value)
        return insert(v,pnode->right,pnode,false);
    else
        return false;
}

template <typename T> treenode<T>* SearchTree<T>::find(T v,treenode<T>* root)
{
    if(root == NULL)
        return NULL;
    if(root->value == v)
        return root;
    else if(root->value < v)
        return find(v,root->right);
    else
        return find(v,root->left);
}

template <typename T> bool SearchTree<T>::Delete(T v)
{
    treenode<T>* del = this->find(v,root);
    if(del == NULL)
        return false;
    else
        Delete(del->value,root);

    return true;
}

template <typename T> treenode<T>* SearchTree<T>::Delete(T v,treenode<T>* root)
{
    if(v > root->value)
        root->right = Delete(v,root->right);
    else if(v < root->value)
        root->left = Delete(v,root->left);
    else if(root->left && root->right)
    {
        treenode<T>* temp = this->findMin(root->right);
        root->value = temp->value;
        root->right = Delete(root->value,root->right);
    }
    else
    {
        if(root->right != NULL)
        {
            treenode<T>* temp = root;
            root = root->right;
            delete temp;
        }
        else
        {
            delete root;
            root = NULL;
        }
    }
    return root;
}

template <typename X> std::string printTree(treenode<X>* root)
{
    std::string res;
    std::stringstream ss;

    if(root == NULL)
        res = "";
    else
    {
        res += printTree(root->left);
        ss << root->value << " ";
        res += ss.str();
        res += printTree(root->right);
    }

    return res;
}

template <typename X> std::ostream& operator<<(std::ostream& os,const SearchTree<X>& st)
{
    std::string res;

    res = printTree(st.root);
    os << res;
    return os;
}

#endif // TREE_H
