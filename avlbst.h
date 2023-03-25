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

		void rrotation (AVLNode<Key, Value>* parent);
		void lrotation (AVLNode<Key, Value>* parent);

		void insertFix(AVLNode<Key, Value>* grandparent, AVLNode<Key, Value>* parent);
		void removeFix(AVLNode<Key, Value>* theNode, int diff);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
        // TODO

    AVLNode<Key,Value> *newNode = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
    if (this->root_ == NULL) {
        this->root_ = newNode;
        return;
    }

	AVLNode<Key,Value> *curNode = static_cast<AVLNode<Key,Value>*>(this->root_);
	AVLNode<Key,Value> *curPar = nullptr;


	while (curNode != nullptr)
	{
		if (new_item.first < curNode->getKey())
		{
			curPar = curNode;
			curNode = curNode->getLeft();
		}
		else if (new_item.first > curNode->getKey())
		{
			curPar = curNode;
			curNode = curNode->getRight();
		}
		else
		{
			curNode->setValue(new_item.second);
			return;
		}
	}



	if (curPar == nullptr)
	{
		this->root_ = newNode;
	}
	else if(new_item.first < curPar->getKey())
	{
		curPar->setLeft(newNode);
	}
	else
	{
		curPar->setRight(newNode);
	}

    newNode->setParent(curPar);

    if (curPar->getBalance() == -1 || curPar->getBalance() == 1)
    {
        curPar->setBalance(0);
    }
    else
    {
        if (curPar->getLeft() == newNode)
        {
            curPar->setBalance(-1);
        }
        else 
        {
            curPar->setBalance(1);
        }
        insertFix(curPar, newNode);
    }


}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* theNode) 
{
    
    if (parent == nullptr || parent->getParent() == nullptr)
    {
        return;
    }

    AVLNode<Key, Value>* grandparent = parent->getParent();

    //left child
    if (grandparent->getLeft() == parent)
    {
        grandparent->updateBalance(-1);

        if (grandparent->getBalance() == 0)
        {
            return;
        }
        else if (grandparent->getBalance() == -1)
        {
            insertFix(grandparent, parent);
            return;
        }
        else if (grandparent->getBalance() == -2)
        {
            //zig zig, if node is to the left of parent
            if (parent->getLeft() == theNode)
            {
                rrotation(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }
            //zigzag, if node is to the right of parent
            else
            {
                lrotation(parent);
                rrotation(grandparent);

            /*. - Case 3a: b(n) == -1 then b(p) = 0; b(g) = +1; b(n) = 0;
                – Case 3b: b(n) == 0 then b(p) = 0; b(g) = 0; b(n) = 0;
                – Case 3c: b(n) == +1 then b(p)= -1; b(g) = 0; b(n) = 0; */

                if (theNode->getBalance() == -1)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                    theNode->setBalance(0);
                }
                else if (theNode->getBalance() == 0)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    theNode->setBalance(0);
                }
                else //if (theNode->getBalance() == 1)
                {
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                    theNode->setBalance(0);
                }


            }
        }
    }
    //right child
    else
    {
        grandparent->updateBalance(1);

        if (grandparent->getBalance() == 0)
        {
            return;
        }
        else if (grandparent->getBalance() == 1)
        {
            insertFix(grandparent, parent);
            return;
        }
        else if (grandparent->getBalance() == 2)
        {
            //zig zig, if node is to the left of parent
            if (parent->getRight() == theNode)
            {
                lrotation(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }
            //zigzag, if node is to the right of parent
            else
            {
                rrotation(parent);
                lrotation(grandparent);

            /*. - Case 3a: b(n) == -1 then b(p) = 0; b(g) = +1; b(n) = 0;
                – Case 3b: b(n) == 0 then b(p) = 0; b(g) = 0; b(n) = 0;
                – Case 3c: b(n) == +1 then b(p)= -1; b(g) = 0; b(n) = 0; */

                if (theNode->getBalance() == 1)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                    theNode->setBalance(0);
                }
                else if (theNode->getBalance() == 0)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    theNode->setBalance(0);
                }
                else //if (theNode->getBalance() == -1)
                {
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                    theNode->setBalance(0);
                }


            }
        }
    }

}

//LEFT Rotation Helper
template<class Key, class Value>
void AVLTree<Key, Value>::lrotation (AVLNode<Key, Value>* theNode)
{
	//the GOAL:
	// grandparent -> theNode -> newParent
	// after function:
	// grandparent->
	//      <- newParent 
	// theNode


	//get the right child of the input node
	AVLNode<Key, Value>* newParent = theNode->getRight();

	//get the parent of the input node
	AVLNode<Key, Value>* grandparent = theNode->getParent();

	//set the parent of newParent to the parent of the input node
	newParent->setParent(grandparent);

	//if the input node is the root of the tree, set the root to newParent
    if (theNode->getParent() == NULL) 
		{
      this->root_ = theNode->getRight();
    }

		//otherwise, update the appropriate child pointer of the parent node
    else if (grandparent->getRight() == theNode) 
		{
      grandparent->setRight(newParent);
    }
    else 
		{
      grandparent->setLeft(newParent);
    }

		//get the left child of newParent
    AVLNode<Key, Value>* newRightChild = newParent->getLeft();

    //set the left child of newParent to the input node
    newParent->setLeft(theNode);

    //set the parent of the input node to newParent
    theNode->setParent(newParent);

    //set the right child of the input node to the left child of newParent
    theNode->setRight(newRightChild);

    //if the left child of newParent exists, set its parent to the input node
    if (newRightChild != NULL) 
		{
      newRightChild->setParent(theNode);
		}
}

//RIGHT Rotation Helper
template<class Key, class Value>
void AVLTree<Key, Value>::rrotation (AVLNode<Key, Value>* theNode)
{
	//the GOAL:
	//      <- grandparent
	//   <- theNode
	// newParent

	// after function:
	//     <- grandparent
	// newParent ->
	//           theNode

	//get the right child of the input node
	AVLNode<Key, Value>* newParent = theNode->getLeft();

	 //get the parent of the input node
	AVLNode<Key, Value>* grandparent = theNode->getParent();

	//set the parent of newParent to the parent of the input node
	newParent->setParent(grandparent);

	//if the input node is the root of the tree, set the root to newParent
    if (theNode->getParent() == NULL) 
		{
      this->root_ = theNode->getLeft();
    }

		//otherwise, update the appropriate child pointer of the parent node
    else if (grandparent->getRight() == theNode) 
		{
      grandparent->setRight(newParent);
    }
    else 
		{
      grandparent->setLeft(newParent);
    }

		//get the right child of newParent
    AVLNode<Key, Value>* newLeftChild = newParent->getRight();

    //set the right child of newParent to the input node
    newParent->setRight(theNode);

    //set the parent of the input node to newParent
    theNode->setParent(newParent);

    //set the left child of the input node to the right child of y
    theNode->setLeft(newLeftChild);

    //if the right child of y exists, set its parent to the input node
    if (newLeftChild != NULL) 
	{
      newLeftChild->setParent(theNode);
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
    // TODO
		AVLNode<Key,Value> *theNode = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));

		//if node not found, return
		if (theNode == nullptr)
		{
			return;
		}

		//if node has two children, swap with predecessor
		if (theNode->getLeft() != NULL && theNode->getRight() != NULL)
		{
			AVLNode<Key,Value> *pred = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key, Value>::predecessor(theNode));
			nodeSwap(theNode, pred);
		}
		
		AVLNode<Key,Value> *child;

		if(theNode->getLeft() != nullptr)
		{
			child = theNode->getLeft();
		}
		else
		{
			child = theNode->getRight();
		}

		int diff;
		AVLNode<Key, Value> *parent = theNode->getParent();

		if(child != NULL)
		{
			child->setParent(parent);
		}

		if(parent == NULL)
		{
			this->root_ = child;
		}
		else if(theNode == parent->getLeft())
		{
			parent->setLeft(child);
			diff = 1;
		}
		else
		{
			parent->setRight(child);
			diff = -1;
		}
		

		
		delete theNode;


		removeFix(parent, diff);
		
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* theNode, int diff) 
{

	if (theNode == nullptr)
	{
		return;
	}

	AVLNode<Key,Value> *parent = theNode->getParent();
	AVLNode<Key,Value> *child;

	int ndiff = -1;
  if ( (parent != NULL) && (theNode == parent->getLeft()) )
	{
      ndiff = 1;
  }

	
	if (theNode->getBalance() + diff == -2)
		{
			child = theNode->getLeft();

			//zig zig
			if (child->getBalance() == -1)
			{
				rrotation(theNode);
				theNode->setBalance(0);
				child->setBalance(0);
				removeFix(parent, ndiff);
			}
			//zig zig
			else if (child->getBalance() == 0)
			{
				rrotation(theNode);
				theNode->setBalance(-1);
				child->setBalance(1);
				return;
			}
			//zig zag
			else //if (child->getBalance() == 1)
			{
				AVLNode<Key,Value> *g = child->getRight();
				lrotation(child);
				rrotation(theNode);

				if (g->getBalance() == 1)
				{
          theNode->setBalance(0);
          child->setBalance(-1);
          g->setBalance(0);
        }
        if(g->getBalance() == 0)
				{
          theNode->setBalance(0);
          child->setBalance(0);
          g->setBalance(0);
        }
        if (g->getBalance() == -1)
				{
          theNode->setBalance(1);
          child->setBalance(0);
          g->setBalance(0);
        }

				removeFix(theNode->getParent(),ndiff);
			}
		}
		else if (theNode->getBalance() + diff == -1)
		{
			theNode->setBalance(-1);
			return;
		}
		else //if (theNode->getBalance() + diff == 0)
		{
			theNode->setBalance(0);
			removeFix(theNode->getParent(), ndiff);
			return;
		}
	
	//else if (diff == 1)

		if (theNode->getBalance() + diff == 2)
		{
			child = theNode->getRight();

			//zig zig
			if (child->getBalance() == 1)
			{
				lrotation(theNode);
				theNode->setBalance(0);
				child->setBalance(0);
				removeFix(theNode->getParent(), ndiff);
			}
			//zig zig
			else if (child->getBalance() == 0)
			{
				lrotation(theNode);
				theNode->setBalance(1);
				child->setBalance(-1);
				return;
			}
			//zig zag
			else //if (child->getBalance() == -1)
			{
				AVLNode<Key,Value> * g = child->getLeft();
				rrotation(child);
				lrotation(theNode);

				if (g->getBalance() == -1)
				{
          theNode->setBalance(0);
          child->setBalance(-1);
          g->setBalance(0);
        }
        if(g->getBalance() == 0)
				{
          theNode->setBalance(0);
          child->setBalance(0);
          g->setBalance(0);
        }
        if (g->getBalance() == -1)
				{
          theNode->setBalance(1);
          child->setBalance(0);
          g->setBalance(0);
        }

				removeFix(parent,ndiff);
			}
		}
		else if (theNode->getBalance() + diff == 1)
		{
			theNode->setBalance(1);
		}
		else //if (theNode->getBalance() + diff == 0)
		{
			theNode->setBalance(0);
			removeFix(theNode->getParent(), ndiff);
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
