#ifndef AVLTREE_H
#define AVLTREE_H

#include "std.h"

template <typename T> struct AVLNode
{
    AVLNode* left;
    AVLNode* right;
    T value;
    int height;
    AVLNode(AVLNode* left = NULL, AVLNode* right = NULL, T value = NULL, int height = 0)
    {
        this->left = left;
        this->right = right;
        this->value = value;
        this->height = height;
    }
};

template <typename T> class AVLTree
{
public:
    AVLTree();
    virtual ~AVLTree();
    void insert(T x);
private:
    AVLNode<T>* root;

    int Height(AVLNode<T>*);
    AVLNode<T>* insert(T, AVLNode<T>*);
    AVLNode<T>* SingleRotateWithLeft(AVLNode<T>*);
    AVLNode<T>* SingleRotateWithRight(AVLNode<T>*);
    AVLNode<T>* DoubleRotateWithLeft(AVLNode<T>*);
    AVLNode<T>* DoubleRotateWithRight(AVLNode<T>*);
};

template <typename T> int AVLTree<T>::Height(AVLNode<T>* node)
{
    if (node == NULL)
        return -1;
    else
        return node->height;
}

template <typename T> AVLNode<T>* AVLTree<T>::insert(T x, AVLNode<T>* root)
{
    if (root == NULL)
    {
        root = new AVLNode<T>();
        root->value = x;
    }
    else if (x < root->value)
    {
        root->left = insert(x, root->left);
        if (Height(root->left) - Height(root->right) == 2)
        {
            if (x < root->left->value)
                root = SingleRotateWithLeft(root);
            else
                root = DoubleRotateWithLeft(root);
        }
    }
    else if (x > root->value)
    {
        root->right = insert(x, root->right);
        if (Height(root->right) - Height(root->left) == 2)
        {
            if (x > root->right->value)
                root = SingleRotateWithRight(root);
            else
                root = DoubleRotateWithRight(root);
        }
    }

    root->height = (root->left->height > root->right->height) ? root->left->height + 1 : root->right->height + 1;
    return root;
}

template <typename T> AVLNode<T>* AVLTree<T>::DoubleRotateWithLeft(AVLNode<T>* root)
{
    root->right = SingleRotateWithLeft(root->right);
    return SingleRotateWithRight(root);
}

template <typename T> AVLNode<T>* AVLTree<T>::DoubleRotateWithRight(AVLNode<T>* root)
{
    root->left = SingleRotateWithRight(root->left);
    return SingleRotateWithLeft(root);
}

template <typename T> AVLNode<T>* AVLTree<T>::SingleRotateWithLeft(AVLNode<T>* root)
{
    AVLNode<T>* left = root->left;
    root->left = left->right;
    left->right = root;

    root->height = (Height(root->left) > Height(root->right))?Height(root->left) + 1 : Height(root->right) + 1;
    left->height = (Height(left->left) > Height(left->right))?Height(left->left) + 1 : Height(left->right) + 1;

    return left;
}

template <typename T> AVLNode<T>* AVLTree<T>::SingleRotateWithRight(AVLNode<T>* root)
{
    AVLNode<T>* ri = root->right;
    root->right = ri->left;
    ri->left = root;

    root->height = (Height(root->left) > Height(root->right))?Height(root->left) + 1 : Height(root->right) + 1;
    ri->height = (Height(ri->left) > Height(ri->right))?Height(ri->left) + 1 : Height(ri->right) + 1;

    return ri;
}

#endif
