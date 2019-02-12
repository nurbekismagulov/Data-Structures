

#ifndef ASSIGNMENT3_HEAPPRIORITYQUEUE_H
#define ASSIGNMENT3_HEAPPRIORITYQUEUE_H

// You are *not* allowed to include other header files

#include <iostream>
#include <stdexcept>
#include "ArrayCompleteTree.h"

using namespace std;


/*
 * This is a generic less-than comparator that relies on operator<() to
 * compare two objects. This generalizes the LeftRight class on Page 326
 * in the textbook.
 */
template<typename T>
class LessThanComparator {
public:
  bool operator()(const T& a, const T& b) const { return a < b; }
};


/*
 * A heap-based priority queue based on ArrayCompleteTree.
 * See Section 8.3.4 in the textbook for more information.
 *
 * Notice that you are *not* allowed to define any other member variables in this class.
 */
template<typename E, typename C>
class HeapPriorityQueue {

public:
  typedef typename ArrayCompleteTree<E>::Position Position;

public:

  HeapPriorityQueue();
  HeapPriorityQueue(const HeapPriorityQueue& q);
  int size() const;
  bool empty() const;
  Position insert(const E& e);
  const E& min();
  void removeMin();
  void remove(const Position& u);
  Position replace(const Position& p, const E& e);


private:

  void bubbleUp(const Position& u);
  void bubbleDown(const Position& u);

private:

  ArrayCompleteTree<E> T;  // the array-based complete tree
  C isLess;                // the less-than comparator

};

template<typename E, typename C>
HeapPriorityQueue<E,C>::HeapPriorityQueue(){}

template<typename E, typename C>
HeapPriorityQueue<E,C>::HeapPriorityQueue(const HeapPriorityQueue& q) : T(q.T) {}

template<typename E, typename C>
int HeapPriorityQueue<E,C>::size() const{
  return T.size();
}

template<typename E, typename C>
bool HeapPriorityQueue<E,C>::empty() const{
  return size() == 0;
}

template<typename E, typename C>
const E& HeapPriorityQueue<E,C>::min(){
  if(empty())
    throw runtime_error("no min element in empty heap");
  return *(T.root());
}

template<typename E, typename C>
typename HeapPriorityQueue<E,C>::Position
HeapPriorityQueue<E,C>::insert(const E& e){
  T.addLast(e);
  Position v = T.last();
  bubbleUp(v);
  return v;
}

template<typename E, typename C>
void HeapPriorityQueue<E,C>::removeMin(){
  if(empty())
    throw runtime_error("no min element in empty heap");
  remove(T.root());
}

template<typename E, typename C>
void HeapPriorityQueue<E,C>::remove(const Position& u){
  if(empty())
    throw runtime_error("can not remove from empty heap");
  if(u == T.last()){
    T.removeLast();
    return;
  }
  Position v = T.last();
  T.swap(u, v);
  T.removeLast();
  bubbleDown(v);
  bubbleUp(v);
}

template<typename E, typename C>
void HeapPriorityQueue<E,C>::bubbleUp(const Position& u){
  while(!T.isRoot(u)){
    Position v = T.parent(u);
    if(!isLess(*u, *v))
      break;
    T.swap(u, v);
  }
}

template<typename E, typename C>
void HeapPriorityQueue<E,C>::bubbleDown(const Position& u){
  while(T.hasLeft(u)){
    Position v = T.left(u);
    if(T.hasRight(u) && isLess(*(T.right(u)), *v)){
      v = T.right(u);
    }
    if(isLess(*v, *u)){
      T.swap(u,v);
    }
    else break;
  }
}

template<typename E, typename C>
typename HeapPriorityQueue<E,C>::Position
HeapPriorityQueue<E,C>::replace(const Position& p, const E& e){
  *p = e;
  bubbleDown(p);
  bubbleUp(p);
  return p;
}

























#endif //ASSIGNMENT3_HEAPPRIORITYQUEUE_H
