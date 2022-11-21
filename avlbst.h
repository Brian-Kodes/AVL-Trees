#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

  //helpers 
  //rotate left rotate right
  //remove fix insert fix
    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    virtual void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);
    virtual void removeFix(AVLNode<Key,Value>* node, int diff);
    virtual void rotateLeft(AVLNode<Key,Value>* p);
    virtual void rotateRight(AVLNode<Key,Value>* parent);
    // Add helper functions here


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key,Value>::rotateLeft(AVLNode<Key,Value>* p)
{
  AVLNode<Key, Value>* current = p -> getRight();
  AVLNode<Key, Value>* GrandParent = p -> getParent();
  current -> setParent(GrandParent);
  if (GrandParent == NULL)
  {
    this -> root_ = current;
  }
  else if (GrandParent -> getRight() == p)
  {
    GrandParent -> setRight(current);
  }
  else
  {
    GrandParent -> setLeft(current);
  }
  AVLNode<Key,Value>* temp = current -> getLeft();
  current -> setLeft(p);
  p -> setParent(current);
  p -> setRight(temp);
  if (temp != NULL)
  {
    temp -> setParent(p);
  }
}


template<class Key, class Value>
void AVLTree<Key,Value>::rotateRight(AVLNode<Key,Value>* parent)
{
  AVLNode<Key, Value>* GrandParent = parent -> getParent();
  AVLNode<Key, Value>* current = parent -> getLeft();
  current -> setParent(GrandParent);
  if (GrandParent == NULL)
  {
    this -> root_ = current;
  }
  else if (GrandParent -> getRight() == parent)
  {
    GrandParent -> setRight(current);
  }
  else
  {
    GrandParent -> setLeft(current);
  }
  AVLNode<Key,Value>* temp = current -> getRight();
  current -> setRight(parent);
  parent -> setParent(current);
  parent -> setLeft(temp);
  if (temp != NULL)
  {
    temp -> setParent(parent);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child)
{
  if (parent == NULL || parent -> getParent() == NULL)
  {
    return;
  }
  AVLNode<Key,Value>* GrandParent = parent -> getParent();
  if (parent == GrandParent -> getLeft())
  {
    GrandParent -> updateBalance(-1); 
    if (GrandParent-> getBalance() == 0)
    {
      return;
    }
    if (GrandParent -> getBalance() == -1)
    {
      insertFix(GrandParent, parent);
      return;
    }
    //else
    //{
      if (child == parent -> getLeft())
      {
        rotateRight(GrandParent);
        parent -> setBalance(0);
        GrandParent -> setBalance(0);
      }
      else
      {
        rotateLeft(parent);
        rotateRight(GrandParent);
      }
        if (child -> getBalance() == -1)
        {
          parent -> setBalance(0);
          GrandParent -> setBalance(1);
          child -> setBalance(0);
        }
        else if (child -> getBalance() == 0)
        {
          parent -> setBalance(0);
          GrandParent -> setBalance(0);
          child -> setBalance(0);
        }
        else
        {
          parent -> setBalance(-1);
          GrandParent -> setBalance(0);
          child -> setBalance(0);
        }
      //}
    }
  else
  {
    GrandParent -> updateBalance(1);
    if (GrandParent-> getBalance() == 0)
    {
      return;
    }
    if (GrandParent -> getBalance() == 1)
    {
      insertFix(GrandParent, parent);
      return;
    }
    //else
    //{
      if (child == parent -> getRight())
      {
        rotateLeft(GrandParent);
        parent -> setBalance(0);
        GrandParent -> setBalance(0);
      }
      else
      {
        rotateRight(parent);
        rotateLeft(GrandParent);
      }

        if (child -> getBalance() == 1)
        {
          parent -> setBalance(0);
          GrandParent -> setBalance(-1);
          child -> setBalance(0);
        }
        else if (child -> getBalance() == 0)
        {
          parent -> setBalance(0);
          GrandParent -> setBalance(0);
          child -> setBalance(0);
        }
        else
        {
          parent -> setBalance(1);
          GrandParent -> setBalance(0);
          child -> setBalance(0);
        }
      
    //}
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
  AVLNode<Key, Value>* node = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
  node -> setBalance(0);
  if(this -> root_ == NULL)
  {
    this -> root_ = node;
    return;
  }
  AVLNode<Key, Value>* current = (AVLNode<Key,Value>*) this->root_;
  AVLNode<Key, Value>* p = NULL;
  //BinarySearchTree<Key,Value>::insert(new_item);
    //AVLNode<Key, Value> searchNode = BinarySearchTree<Key,Value>::internalFind(new_item.first);
  //used to be a if (this -> root != NULL);
    while (current != NULL)
    {
      p = current;
      //current node to compare w node in tree
      // another node to point at parent
      if (new_item.first < current -> getKey())
      {
        current = current -> getLeft();
      }
      else
      {
        current = current -> getRight();
      }
    }
    AVLNode<Key, Value>* toInsert = new AVLNode<Key,Value>(new_item.first, new_item.second, p);
    if (new_item.first == p -> getKey())
    {
      p -> setValue(new_item.second);
      return;
    }
    if (new_item.first > p -> getKey())
    {
      p->setRight(toInsert);
    }
    else
    {
      p->setLeft(toInsert);
    }
    if (p -> getBalance() == -1 || p -> getBalance() == 1)
    {
      p -> setBalance(0);
    }
    else
    {
      if (toInsert == p -> getLeft())
      {
        p -> setBalance(-1);
        toInsert -> setBalance(0);
      }
      else if (toInsert == p -> getRight())
      {
        p -> setBalance(1);
        toInsert -> setBalance(0);
      }
      //p -> updateBalance (balval);
      insertFix(p, toInsert);
    }
    // {
    //   root -> getLeft() = insert(root->getLeft(), new_item.first);
    // }

}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */


template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* node, int diff)
{
  int nextDiff;
  if (node == NULL)
  {
    return;
  }
  AVLNode<Key,Value>* parent = node -> getParent();
  if (parent != NULL)
  {
    if (parent -> getLeft() == node)
    {
      nextDiff = 1;
    }
    else if (parent -> getRight() == node)
    {
      nextDiff = -1;
    }
  }
  if (diff == -1)
  {
    if (node -> getBalance() + diff == -2)
    {
      AVLNode<Key, Value>* child = node -> getLeft();
      if (child -> getBalance() == -1)
      {
        rotateRight(node);
        node -> setBalance(0);
        child -> setBalance(0);
      }
      else if (child -> getBalance() == 0)
      {
        rotateRight(node);
        node -> setBalance(-1);
        child -> setBalance(0);
      }
      else if (child -> getBalance() == 1)
      {
        AVLNode<Key, Value>* g = child -> getRight();
        rotateLeft(child);
        rotateRight(node);
        if (g -> getBalance() == 1)
        {
          node -> setBalance(0);
          child -> setBalance(-1);
          g -> setBalance(0);
        }
        else if (g -> getBalance() == 0)
        {
          node -> setBalance(0);
          child -> setBalance(0);
          g -> setBalance(0);
        }
        else if (g -> getBalance() == 1)
        {
          node -> setBalance(1);
          child -> setBalance(0);
          g -> setBalance(0);
        }
        removeFix(parent, nextDiff);
      }
    }
    else if (node -> getBalance() + diff == -1)
    {
      node -> setBalance(-1);
    }
    else if (node -> getBalance() + diff == 0)
    {
      node -> setBalance(0);
      removeFix(parent, nextDiff);
    }
  }
  else
  {
    if (node -> getBalance() + diff == 2)
    {
      AVLNode<Key, Value>* child = node -> getRight();
      if (child -> getBalance() == 1)
      {
        rotateLeft(node);
        node -> setBalance(0);
        child -> setBalance(0);
        removeFix(parent, nextDiff);
      }
      else if (child -> getBalance() == 0)
      {
        rotateLeft(node);
        node -> setBalance(1);
        child -> setBalance(0);
      }
      else if (child -> getBalance() == -1)
      {
        AVLNode<Key, Value>* g = child -> getLeft();
        rotateRight(child);
        rotateLeft(node);
        if (g -> getBalance() == 1)
        {
          node -> setBalance(0);
          child -> setBalance(1);
          g -> setBalance(0);
        }
        else if (g -> getBalance() == 0)
        {
          node -> setBalance(0);
          child -> setBalance(0);
          g -> setBalance(0);
        }
        else if (g -> getBalance() == -1)
        {
          node -> setBalance(-1);
          child -> setBalance(0);
          g -> setBalance(0);
        }
        removeFix(parent, nextDiff);
      }
    }
    else if (node -> getBalance() + diff == 1)
    {
      node -> setBalance(1);
    }
    else if (node -> getBalance() + diff == 0)
    {
      node -> setBalance(0);
      removeFix(parent, nextDiff);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  int diff;
  AVLNode<Key,Value>* nodetoBeRemoved = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
  if (nodetoBeRemoved == NULL)
  {
    return;
  }
  if (nodetoBeRemoved -> getLeft() != NULL && nodetoBeRemoved -> getRight () != NULL)
  {
    AVLNode<Key,Value>* predecessorNode = static_cast<AVLNode<Key,Value>*>(this->predecessor(nodetoBeRemoved));
    nodeSwap(predecessorNode, nodetoBeRemoved);
  }
  AVLNode<Key,Value>* removeP = nodetoBeRemoved -> getParent();
  AVLNode<Key,Value>* child = nodetoBeRemoved -> getLeft();
  if (nodetoBeRemoved -> getRight() != NULL)
  {
    child = nodetoBeRemoved -> getRight();
  }
  /*if (nodetoBeRemoved -> getLeft() != NULL || nodetoBeRemoved->getRight() != NULL)
  {
    if (nodetoBeRemoved -> getLeft() != NULL)
    {
      child = nodetoBeRemoved -> getLeft();
    }
    else if (nodetoBeRemoved -> getRight() != NULL)
    {
      child = nodetoBeRemoved -> getRight();
    }
  }*/

  if (child != NULL)
  {
    child -> setParent(removeP);
  }
  if (removeP != NULL)
  {
    if (removeP -> getLeft() == nodetoBeRemoved)
    {
      removeP -> setLeft(child);
      diff = 1;
    }
    else if (removeP -> getRight() == nodetoBeRemoved)
    {
      removeP -> setRight(child);
      diff = -1;
    }
  }
  else
  {
    this -> root_ = child;
  }
  delete nodetoBeRemoved;
  removeFix(removeP, diff);
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
