/*
 * avl.h
 *
 *  Created on: May 14, 2014
 *      Author: paulcarmody
 */

#ifndef AVL_H_
#define AVL_H_

#pragma once
#include <sstream>
#include <iostream>
#include <algorithm>
#include "mylinkedlist.h"

using namespace std;

template <typename Titem> class AVLtree {
 public:
	//
	// define Tnode
	//

  class Tnode {
  public:
    Titem item;
    Tnode* left;
    Tnode* right;
    int height;
  public:

    // Tnode constructor
    Tnode (Titem x, Tnode* l=NULL, Tnode* r=NULL)
      {
        item = x;
        left = l;
        right = r;
        height = 1; // start height at 1
      }

    // Tnode destructor
    ~Tnode()
      {}
    void print(int indent)
    {
    	if(left)
    		left->print(indent+1);
    	for(int i=0; i<indent; i++)
    		cout << "  ";
    	cout << item << "\n";
    	if(right)
    		right->print(indent+1);
    }
  };

  //
  // define AVLtree
  //

  Tnode* root;
  int size;

  // AVL tree constructor
  AVLtree (int s=0) // initialize a new tree with a null root
    {
      root = NULL;
      size = s;
    }

  ~AVLtree ()
    {
      this->clear();
    }

  int getHeight (Tnode* n)
  {
    if (n == NULL)
      return 0;
    else
      return n->height;
  }

  Titem getItem (Tnode* n)
  {
    if (n == NULL)
      return "NULL";
    else
      return n->item;
  }

  Tnode* setHeight(Tnode* n)
  {
    int LH = getHeight(n->left);
    int RH = getHeight(n->right);
    n->height = ((max(LH, RH)) + 1);
    //n->height = LH+RH+1;
    return n;
  }

  // single right rotate
  Tnode* SRR (Tnode* n) // where n is the Tnode that is out of balance
  {
    Tnode* k = n->left;
// make the parent of n point to k instead of n -- done by recursion
// make n's left child pointer point to k's right child
    n->left = k->right;
// make k's right child pointer point to n
    k->right = n;
    n = setHeight(n);
    k = setHeight(k);
    return k;
  }

  // single left rotate
  Tnode* SLR (Tnode* n)
  {
    Tnode* k = n->right;
    // make the parent of n point to k instead of n
// make n's right child pointer point to k's left child
    n->right = k->left;
// make k's left child pointer point to n
    k->left = n;
    n = setHeight(n);
    k = setHeight(k);
    return k;
  }

  // double right rotate
  Tnode* DRR (Tnode* n)
  {
    n->left = SLR(n->left);
    n = SRR(n);
    return n;
  }

  // double left rotate
  Tnode* DLR (Tnode* n)
  {
    n->right = SRR(n->right);
    n = SLR(n);
    return n;
  }

  // BST functions
  Tnode* BSTinsert (Tnode* n, Titem item) // n is the new root of the subtree
  {
    if (n == NULL) // found the right spot to insert
      {
        size++;
        return new Tnode(item); // MEMORY LEAK
      } // make a new Tnode with the item
    else if (n->item == item) // already exists
      return n;
    else if (n->item < item)
      n->right = BSTinsert (n->right, item); // insert to the right subtree
    else
      n->left = BSTinsert (n->left, item); // insert to the left subtree
    n = setHeight(n);
    n = balance(n); // balance the tree
    return n;
  }

  Tnode* BSTremove (Tnode* n, Titem item)
  {
    if (n == NULL) // got down to the end without finding the object to be removed
      {
        return NULL; // CHANGED FROM NULL
      } // item not found
    if (n->item == item) // item found at current Tnode
      {
        if (getHeight(n) == 1) // leaf Tnode
          {
            // "BSTremove: leaf Tnode" << endl;
            delete n;
            size--; // remove a Tnode from the tree
            return NULL;
          }
        else if ((n->right == NULL)&&(n->left != NULL)) // left child only
          {
            size--;
            Tnode* temp = n;
            n = n->left;
            delete (temp);
            return n;
          }
        else if ((n->left == NULL)&&(n->right != NULL)) // right child only
          {
            size--;
            Tnode* temp = n;
            n = n->right;
            delete (temp);
            return n;
          }
        else if ((n->left != NULL)&&(n->right != NULL)) // two children
          {
            Titem temp = findMin(n->right);
            n->item = temp; // set the current Tnode to contain that smallest item
            n->right = BSTremove (n->right, temp);
          } // remove the smallest item from the right subtree
      }
    else if (n->item < item) // the item we are looking for is bigger than the one in the current Tnode
      n->right = BSTremove (n->right, item);// remove from the right subtree
    else if (n->item >= item)
      n->left = BSTremove (n->left, item); // remove from left subtree
    n = setHeight(n);
    n = balance(n);
    return n;
  }

  Titem findMin(Tnode* n)
  {
    // find the smallest thing in this subtree
    if (n->left == NULL) // gone down as far as possible
      return n->item;
    else
      return findMin(n->left); // go left
  }

  Titem *BSTfind (Tnode* n, Titem item)
  {
    if (n == NULL) // reached a null Tnode
      return NULL;
    if (item < n->item)
      return BSTfind (n->left, item);
    else if (item > n->item)
      return BSTfind (n->right, item);
    else
      return &n->item; // item must be found
  }

  void BSTclear (Tnode* n)
  {
    if (n->left != NULL)
      BSTclear(n->left);
    if (n->right != NULL)
      BSTclear(n->right);
    delete n;
  }

  Tnode* balance(Tnode* n)
  {
    // check if right or left ones are null?
    if (getHeight(n->left) - getHeight(n->right) > 1) // out of balance and left side is heavier
      n = balRight(n);
    else if (getHeight(n->right) - getHeight(n->left) > 1) // right side heavier
      n = balLeft(n);
    return n;
  }

  Tnode* balRight (Tnode* n)
  {
    if ((getHeight(n->left->right)) > (getHeight(n->left->left)))
      // the "crooked" grandchild is taller than the other one
      {n = DRR(n);
        return n;}
    else
      {n = SRR(n);
        return n;}
  }

  Tnode* balLeft (Tnode* n)
  {
    if ((getHeight(n->right->left)) > (getHeight(n->right->right)))
      n = DLR(n);
    else
      n = SLR(n);
    return n;
  }

 public:
  // clear the whole set
  void clear ()
  {
    if (root != NULL)
      BSTclear(root);
    root = NULL;
    size = 0;
    return;
  }

  // insert into the tree
  void add (const Titem item)
  {
    if (this->find(item))
      return;
    // check if item already exists (using find)
    if ((size == 0)) // nothing in the tree yet
      {
        root = new Tnode(item); // make the root a new Tnode with the item and return
        size++; // add one Tnode to tree
        return;
      }
    else
      {
        root = BSTinsert (root, item); // call the recursive BST function on the root
        return;
      }
    // "AVLtree add: size after: " << size << endl;
  }

  // remove object from tree
  void remove (const Titem item)
  {
    if (size == 0) // nothing is in the tree
      return;
    else
      root = BSTremove (root, item); // call recursive function on root
    return;
  }

  // find object in tree
  Titem *find (const Titem item)
  {
    if (root != NULL)
      return BSTfind(root, item);
    else
      return NULL;
  }

  void myprint()
  {
	  root->print(0);
  }
};
#endif /* AVL_H_ */
