/*
 * AssocArray.h
 *
 *  Created on: May 12, 2014
 *      Author: paulcarmody
 */

#ifndef ASSOCARRAY_H_
#define ASSOCARRAY_H_

#include <iostream>
#include <vector>
#include "avl.h"
using namespace std;

template <class T, class S> class Generic_Assoc_Array {
  private:
	  class Data
	  {
	  public:
		  int		rec_num;
		  S 	name;
		  Data() {};
		  bool operator < (Data &d) { return name < d.name; };
		  bool operator > (Data &d) { return name > d.name; };
		  bool operator == (Data &d) { return name == d.name; };
	  };
	  AVLtree<Data>	tree;
	  vector<T> 	stack;
  public:
	  long Size()
	  {
		  return stack.size();
	  }

	  bool IsItem(S name)
	  {
//		  for(int i=0; i<Size(); i++)
//		  {
//			  if(stack[i].name == name)
//				  return true;
//		  }
//		  return false;
		  Data search;
		  search.name = name;
		  return tree.find(search);
	  }

	  bool AddItem(S name, T data)
	  {
		  if(IsItem(name))
			  return false;
		  Data d;
		  d.name = name;
		  d.rec_num = stack.size();
		  stack.push_back(data);
		  tree.add(d);
		  return true;
	  }

	  void Reset(S name, T data)
	  {
		  Data search;
		  search.name = name;
		  Data *d = tree.find(search);
		  if(d && d->rec_num > 0)
			  stack[d->rec_num] = data;
		  else
			  stack[0] = data;
	  }

	  T& operator [] (S name)
	  {
//		  for(int i=0; i<Size(); i++)
//		  {
//			  if(stack[i].name == name)
//				  return stack[i].data;
//		  }
		  Data search;
		  search.name = name;
		  Data *d = tree.find(search);
		  if(d && d->rec_num > 0)
			  return stack[d->rec_num];
		  return stack[0];
//		  long idx = Size();
//		  Data d;
//		  d.name = name;
//		  stack.push_back(d);
//		  return stack[idx].data;
	  }

//	  S GetItemName(long index)
//	  {
//		  if(index<0)
//			  index = 0;
//		  for(int i=0; i<Size(); i++)
//			  if(i == index)
//				  return stack[i].name;
//		  return "";
//	  }

	  T& operator [] (long index)
	  {
		  if(index < 0)
			  index = 0;
//		  for(int i=0; i<Size(); i++)
//		  {
//			  if(i == index)
//				  return stack[i].data;
//		  }
		  return stack[index];
	  }

	  T& nth (long index)
	  {
		  if(index < 0)
			  index = 0;
//		  for(int i=0; i<Size(); i++)
//		  {
//			  if(i == index)
//				  return stack[i].data;
//		  }
		  return stack[index];
	  }

};

template <class T> class AssocArray : public Generic_Assoc_Array<T, string> {};
template <class T> class IntAssocArray : public Generic_Assoc_Array<T, int> {};

#endif /* ASSOCARRAY_H_ */
