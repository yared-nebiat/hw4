#ifndef AVLBST_H
#define AVLBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor
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
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parentNode, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);
    void rotateLeft(AVLNode<Key,Value>* rootNode);
    void rotateRight(AVLNode<Key,Value>* rootNode);



};


template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key, Value> *rootNode)
{
    AVLNode<Key, Value>* newRoot = rootNode->getRight();
    AVLNode<Key, Value>* temp = rootNode->getParent();

    if (rootNode->getParent() == nullptr) {        
        this->root_ = newRoot;
    }
    else if (temp->getLeft() == rootNode){
        temp->setLeft(newRoot);
    }
    else{
        temp->setRight(newRoot);
    }
    newRoot->setParent(temp);
    
    AVLNode<Key, Value>* rootChild = newRoot->getLeft();

    newRoot->setLeft(rootNode);
    rootNode->setParent(newRoot);
    
    rootNode->setRight(rootChild);
    if (rootChild != nullptr){
        rootChild->setParent(rootNode);
    }   
    
}


template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key, Value> *rootNode)
{
    AVLNode<Key, Value>* newRoot = rootNode->getLeft();
    AVLNode<Key, Value>* temp = rootNode->getParent();

    if (rootNode->getParent() == nullptr) {        
        this->root_ = newRoot;
    }
    else if (temp->getLeft() == rootNode){
        temp->setLeft(newRoot);
    }
    else{
        temp->setRight(newRoot);
    }
    newRoot->setParent(temp);    

    AVLNode<Key, Value>* rootChild = newRoot->getRight();

    newRoot->setRight(rootNode);
    rootNode->setParent(newRoot);

    rootNode->setLeft(rootChild);
    if (rootChild != nullptr){
        rootChild->setParent(rootNode);
    }
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parentNode;
    AVLNode<Key, Value>* node;
    bool right_side; // true if we go right child, false if we go left child
    // if the tree is empty
    if(this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value> (new_item.first, new_item.second, nullptr);
        this->root_->setLeft(nullptr);
        this->root_->setRight(nullptr);
        return;
    }

    while(temp != nullptr) {
        parentNode = temp;
        if(temp->getKey() == new_item.first) {
            temp->setValue(new_item.second);
            return;
        }
        else if(temp->getKey() > new_item.first) {
            right_side = false;  // Moving to the left
            if (temp->getLeft() == nullptr) break;
            temp = temp->getLeft();
        }
        else {
            right_side = true;   // Moving to the right
            if (temp->getRight() == nullptr) break;
            temp = temp->getRight();
        }
    }

    node = new AVLNode<Key, Value> (new_item.first, new_item.second, parentNode);
    node->setBalance(0);

    if(right_side) {
        parentNode->setRight(node);
    }
    else{
        parentNode->setLeft(node);
    }

    if(parentNode->getBalance() == -1 || parentNode->getBalance() == 1) {
        parentNode->setBalance(0);
        return;
    }
    bool is_balanced = false;
    parentNode->setBalance(this->treeHeight(parentNode->getRight(), is_balanced) - this->treeHeight(parentNode->getLeft(), is_balanced));

    insertFix(parentNode, node);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parentNode, AVLNode<Key, Value>* node) {

    if(parentNode == nullptr || parentNode->getParent() == nullptr) return;

    AVLNode<Key, Value>* grandparentNode = static_cast<AVLNode<Key, Value>*>(parentNode->getParent());  

    if(grandparentNode->getLeft() == parentNode) {
        grandparentNode->updateBalance(-1);
        if(grandparentNode->getBalance() == -1) {
            insertFix(grandparentNode, parentNode);
        }
        else if(grandparentNode->getBalance() == -2) {
            if(parentNode->getBalance() == -1) { // Zig-Zig left-left case
                rotateRight(grandparentNode);
                parentNode->setBalance(0);
                grandparentNode->setBalance(0);
            }
            else if(parentNode->getBalance() == 1) { // Zig-Zag left-right case
                rotateLeft(parentNode);
                rotateRight(grandparentNode);

                // Adjust the balance factors based on node balance
                if (node->getBalance() == -1) {
                    parentNode->setBalance(0);
                    grandparentNode->setBalance(1);
                } else if (node->getBalance() == 0) {
                    parentNode->setBalance(0);
                    grandparentNode->setBalance(0);
                } else if (node->getBalance() == 1) {
                    parentNode->setBalance(-1);
                    grandparentNode->setBalance(0);
                }
                node->setBalance(0);
            }
        }        
    }
    else {
        grandparentNode->updateBalance(1);
        if(grandparentNode->getBalance() == 1) {
            insertFix(grandparentNode, parentNode);
        }
        else if(grandparentNode->getBalance() == 2) {
            if(parentNode->getBalance() == 1) { // Zig-Zig right-right case
                rotateLeft(grandparentNode);
                parentNode->setBalance(0);
                grandparentNode->setBalance(0);
            }
            else if(parentNode->getBalance() == -1) { // Zig-Zag right-left case
                rotateRight(parentNode);
                rotateLeft(grandparentNode);

                // Adjust the balance factors based on node balance
                if (node->getBalance() == 1) {
                    parentNode->setBalance(0);
                    grandparentNode->setBalance(-1);
                } else if (node->getBalance() == 0) {
                    parentNode->setBalance(0);
                    grandparentNode->setBalance(0);
                } else if (node->getBalance() == -1) {
                    parentNode->setBalance(1);
                    grandparentNode->setBalance(0);
                }
                node->setBalance(0);
            }
        }   
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if(this->root_ == nullptr || temp == nullptr) {
        return;
    }

    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));

    AVLNode<Key, Value>* parentNode;
    AVLNode<Key, Value>* child;

    int8_t diff;

    // if the node has two children
    if(temp->getLeft() != nullptr && temp->getRight() != nullptr) {
        nodeSwap(temp, pred);
    }

    parentNode = temp->getParent();

    // no children
    if(temp->getLeft() == nullptr && temp->getRight() == nullptr) {
        if(this->root_ == temp) {
            this->root_ = nullptr;
        } else {
            if(parentNode->getLeft() == temp) {
                parentNode->setLeft(nullptr);
                diff = 1;
            } else {
                parentNode->setRight(nullptr);
                diff = -1;
            }
        }
    }

    // only left child
    else if(temp->getLeft() != nullptr && temp->getRight() == nullptr) {
        child = temp->getLeft();
        if(child != nullptr) {
            child->setParent(parentNode);
        }
        if(this->root_ == temp) {
            this->root_ = child;
        }
        else {
            if(parentNode->getLeft() == temp) {
                parentNode->setLeft(child);
                diff = 1;
            }
            else {
                parentNode->setRight(child);
                diff = -1;
            }
        }
    }

    // only right child
    else if(temp->getLeft() == nullptr && temp->getRight() != nullptr) {
        child = temp->getRight();
        if(child != nullptr) {
            child->setParent(parentNode);
        }
        if(this->root_ == temp) {
            this->root_ = child;
        }
        else {
            if(parentNode->getLeft() == temp) {
                parentNode->setLeft(child);
                diff = 1;
            }
            else {
                parentNode->setRight(child);
                diff = -1;
            }
        }
    }

    temp->setParent(nullptr);
    temp->setLeft(nullptr);
    temp->setRight(nullptr);

    delete temp;

    removeFix(parentNode, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff) {
    if(node == nullptr) return;

    AVLNode<Key, Value>* parentNode = static_cast<AVLNode<Key, Value>*>(node->getParent());

    int8_t ndiff;

    if(parentNode != nullptr) {
        if(parentNode->getLeft() == node) {
            ndiff = 1;
        }
        else {
            ndiff = -1;
        }
    }
    AVLNode<Key, Value>* childNode;
    AVLNode<Key, Value>* grandchildNode;

    if(diff == -1) {
        if(node->getBalance() + diff == -2) {
            childNode = node->getLeft();
        
            if(childNode->getBalance() == -1) { // Zig-Zig left-left case
                rotateRight(node);
                node->setBalance(0);
                childNode->setBalance(0);
                removeFix(parentNode, ndiff);
            }
            else if(childNode->getBalance() == 0) { 
                rotateRight(node);
                node->setBalance(-1);
                childNode->setBalance(1);
            }
            else if(childNode->getBalance() == 1) { // Zig-Zag left-right case
                grandchildNode = childNode->getRight();
                rotateLeft(childNode);
                rotateRight(node);

                // Adjust the balance factors based on node balance
                if (grandchildNode->getBalance() == 1) {
                    node->setBalance(0);
                    childNode->setBalance(-1);
                    grandchildNode->setBalance(0);

                } else if (grandchildNode->getBalance() == 0) {
                    node->setBalance(0);
                    childNode->setBalance(0);
                    grandchildNode->setBalance(0);

                } else if (grandchildNode->getBalance() == -1) {
                    node->setBalance(1);
                    childNode->setBalance(0);
                    grandchildNode->setBalance(0);
                }
                removeFix(parentNode, ndiff);
            }
        }
        else if(node->getBalance() + diff == -1) {
            node->setBalance(-1);
            return;
        }
        else if(node->getBalance() + diff == 0) {
            node->setBalance(0);
            removeFix(parentNode, ndiff);
        }
    }
    else {
        if(node->getBalance() + diff == 2) {
            childNode = node->getRight();
        
            if(childNode->getBalance() == 1) { // Zig-Zig right-right case
                rotateLeft(node);
                node->setBalance(0);
                childNode->setBalance(0);
                removeFix(parentNode, ndiff);
            }
            else if(childNode->getBalance() == 0) { 
                rotateLeft(node);
                node->setBalance(1);
                childNode->setBalance(-1);
            }
            else if(childNode->getBalance() == -1) { // Zig-Zag right-left case
                grandchildNode = childNode->getLeft();
                rotateRight(childNode);
                rotateLeft(node);

                // Adjust the balance factors based on node balance
                if (grandchildNode->getBalance() == -1) {
                    node->setBalance(0);
                    childNode->setBalance(1);
                    grandchildNode->setBalance(0);

                } else if (grandchildNode->getBalance() == 0) {
                    node->setBalance(0);
                    childNode->setBalance(0);
                    grandchildNode->setBalance(0);

                } else if (grandchildNode->getBalance() == 1) {
                    node->setBalance(-1);
                    childNode->setBalance(0);
                    grandchildNode->setBalance(0);
                }
                removeFix(parentNode, ndiff);
            }
        }
        else if(node->getBalance() + diff == 1) {
            node->setBalance(1);
            return;
        }
        else if(node->getBalance() + diff == 0) {
            node->setBalance(0);
            removeFix(parentNode, ndiff);
        }
    }

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
