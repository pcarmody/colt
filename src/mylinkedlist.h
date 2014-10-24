/*
 * mylinkedlist.h
 *
 *  Created on: May 15, 2014
 *      Author: paulcarmody
 */

#ifndef MYLINKEDLIST_H_
#define MYLINKEDLIST_H_

#pragma once

#include <sstream>

using namespace std;

template <typename T>

class LinkedList
{
 public:
  class Node
  {
  public:
    T item;
    Node* next;
    Node* prev;

    Node (T item0, Node* next0, Node* prev0)
      {
        item = item0;
        next = next0;
        prev = prev0;
      }

    ~Node()
      {}
  };

  //list
  Node* head;
  Node* tail;
  int size;

  LinkedList ()
    {
      head = NULL;
      tail = NULL;
      size = 0;
    }

  ~LinkedList()
    {
      if (size == 0)
        return;
      Node* temp = head;
      while (temp != NULL)
        {
          head = head->next;
          delete temp;
          temp = head;
        }
    }

  LinkedList& operator =(const LinkedList& list)
    {
      head = list.head;
      tail = list.tail;
      size = list.size;
    }

  void pushBack (const T& item)
  {
    if (size == 0)
      {
        head = new Node (item, NULL, NULL);
        tail = head;
        size++;
        return;
      }
    tail->next = new Node (item, NULL, tail);
    tail = tail->next;
    size++;
    return;
  }


  void pushFront (const T& item)
  {
    if (size == 0)
      {
        head = new Node (item, NULL, NULL);
        tail = head;
        size++;
        return;
      }
    head->prev = new Node (item, head, NULL);
    head = head->prev;
    size++;
    return;
  }

  void pop ()
  {
    if (size == 0)
      return;
    Node* temp = tail;
    tail = tail->prev;
    delete temp;
    size--;
    return;
  }

  T end ()
  {
    T def;
    if (tail != NULL)
      return tail->item;
    else
      return def;
  }

  bool empty ()
  {
    return (size == 0);
  }

};

#endif /* MYLINKEDLIST_H_ */
