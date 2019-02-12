

#ifndef ASSIGNMENT3_ARRAYCOMPLETETREE_H
#define ASSIGNMENT3_ARRAYCOMPLETETREE_H

// You are *not* allowed to include other header files

#include <iostream>
#include <stdexcept>

using namespace std;


/*
 * An array-based, adaptable complete tree with location-aware entries.
 * This implementation is based on Section 8.3 and Section 8.4 in the textbook.
 * The difference is that the underlying data structure is a raw array.
 * When the array runs out of space, the doubling strategy is employed
 * to enlarge the array.
 *
 * Notice that you are *not* allowed to define any other member variables in this class.
 */
template<typename E>
class ArrayCompleteTree{
public:


  class Locator {
    int i;   // the index of the location-aware entry in v
  public:
    Locator(int _i): i(_i){}
    friend class ArrayCompleteTree<E>;
  };


  class LocationAwareEntry {
    E e;             // the element
    Locator *loc;    // a pointer to the locator of this entry
  public:
    LocationAwareEntry(): loc(NULL){}
    LocationAwareEntry(E _e, int i): e(_e) {  loc = new Locator(i); }
    void clear(){ delete loc; loc = NULL; }
    friend class ArrayCompleteTree<E>;
  };


  class Position {
    const ArrayCompleteTree* tree;     // a pointer to the ArrayCompleteTree object that this Position object works for.
    Locator *loc;                       // a pointer to the locator object of the entry pointed by this Position object.
  public:
    Position(): tree(NULL), loc(NULL){}
    Position(const ArrayCompleteTree* _tree, Locator* _loc): tree(_tree), loc(_loc){}
    bool operator==(const Position& p) const{
      if(tree == NULL && p.tree == NULL) return true;
      else if(tree && p.tree && tree == p.tree) return loc == p.loc;
      else return false;
    }
    E& operator*() const {
      if(loc == NULL)
        throw runtime_error("Pos loc is null");
      return (tree->v + loc->i)->e; }
    friend class ArrayCompleteTree<E>;
  };


public:

  ArrayCompleteTree(int _vsize = 10);
  ArrayCompleteTree(const ArrayCompleteTree& t);
  ~ArrayCompleteTree();
  int size() const;
  Position left(const Position& p) const;
  Position right(const Position& p) const;
  Position parent(const Position& p) const;
  bool hasLeft(const Position& p) const;
  bool hasRight(const Position& p) const;
  bool isRoot(const Position& p) const;
  Position root() const;
  Position last() const;
  void addLast(const E& e);
  void removeLast();
  void swap(const Position& p, const Position& q);
  void cleanup();

private:

  LocationAwareEntry* v;   // the array of location-aware entries
  int vsize;               // the size of v
  int n;                   // the number of used entries in v

};

template<typename E>
ArrayCompleteTree<E>::ArrayCompleteTree(int _vsize): vsize(_vsize), n(0) {
  v = new LocationAwareEntry[vsize];
}

template<typename E>
ArrayCompleteTree<E>::ArrayCompleteTree(const ArrayCompleteTree& t){
  vsize = t.vsize;
  n = t.n;
  v = new LocationAwareEntry[vsize];
  for(int i = 0; i < n; i++){
    v[i] = LocationAwareEntry((t.v + i)->e, i);
  }
}

template<typename E>
ArrayCompleteTree<E>::~ArrayCompleteTree(){
  while(n > 0){
    removeLast();
  }
  delete [] v;
  vsize = 0;
}

template<typename E>
int ArrayCompleteTree<E>::size() const{
  return n;
}

template<typename E>
typename ArrayCompleteTree<E>::Position
ArrayCompleteTree<E>::left(const Position& p) const{
  if(p.loc == NULL)
    throw runtime_error("p loc is null");
  if(!hasLeft(p))
    throw runtime_error("p has no left child");
  return Position(this, (v + 2 * (p.loc->i) + 1)->loc);
}

template<typename E>
typename ArrayCompleteTree<E>::Position
ArrayCompleteTree<E>::right(const Position& p) const{
  if(p.loc == NULL)
    throw runtime_error("p loc is null");
  if(!hasRight(p))
    throw runtime_error("p has no right child");
  return Position(this, (v + 2 * (p.loc->i) + 2)->loc);
}

template<typename E>
typename ArrayCompleteTree<E>::Position
ArrayCompleteTree<E>::parent(const Position& p) const{
  if(p.loc == NULL)
    throw runtime_error("p loc is null");
  if(isRoot(p))
    throw runtime_error("p is root no parent");
  return Position(this, (v + (p.loc->i - 1) / 2)->loc);
}

template<typename E>
bool ArrayCompleteTree<E>::hasLeft(const Position& p) const{
  if(p.loc == NULL)
    throw runtime_error("p loc is null");
  return 2 * (p.loc->i) + 1 <= n - 1;
}

template<typename E>
bool ArrayCompleteTree<E>::hasRight(const Position& p) const{
  if(p.loc == NULL)
    throw runtime_error("p loc is null");
  return 2 * (p.loc->i) + 2 <= n - 1;
}

template<typename E>
bool ArrayCompleteTree<E>::isRoot(const Position& p) const{
  if(p.loc == NULL)
    throw runtime_error("p loc is null");
  return p.loc->i == 0;
}

template<typename E>
typename ArrayCompleteTree<E>::Position
ArrayCompleteTree<E>::root() const{
  if(size() == 0)
    throw runtime_error("empty tree no root");
  return Position(this, v->loc);
}

template<typename E>
typename ArrayCompleteTree<E>::Position
ArrayCompleteTree<E>::last() const{
  if(size() == 0)
    throw runtime_error("no last element of empty tree");
  return Position(this, (v + n - 1)->loc);
}

template<typename E>
void ArrayCompleteTree<E>::addLast(const E& e){
  if(n >= vsize){
    vsize = max(1, 2 * vsize);
    LocationAwareEntry* temp = new LocationAwareEntry[vsize];
    for(int i = 0; i < n; i++){
      (temp + i)->e = (v + i)->e;
      (temp + i)->loc = (v + i)->loc;
    }
    delete [] v;
    v = temp;
    temp = NULL;
  }
  v[n] = LocationAwareEntry(e, n);
  n++;
}

template<typename E>
void ArrayCompleteTree<E>::removeLast(){
  if(size() == 0)
    throw runtime_error("empty tree no remove");
  (v + n - 1)->clear();
  n--;
}

template<typename E>
void ArrayCompleteTree<E>::swap(const Position& p, const Position& q){
  if(p.loc == NULL)
    throw runtime_error("p loc is null");
  if(q.loc == NULL)
    throw runtime_error("q loc is null");
  E tempelement = *p;
  *p = *q;
  *q = tempelement;
  int tempindex = p.loc->i;
  p.loc->i = q.loc->i;
  q.loc->i = tempindex;
  Locator* temploc = (v + p.loc->i)->loc;
  (v + p.loc->i)->loc = (v + q.loc->i)->loc;
  (v + q.loc->i)->loc = temploc;
}

template<typename E>
void ArrayCompleteTree<E>::cleanup(){
  if(n < vsize){
    vsize = n;
    LocationAwareEntry* temp = new LocationAwareEntry[vsize];
    for(int i = 0; i < n; i++){
      (temp + i)->e = (v + i)->e;
      (temp + i)->loc = (v + i)->loc;
      delete [] v;
      v = temp;
      temp = NULL;
    }
  }
}































































































#endif //ASSIGNMENT3_ARRAYCOMPLETETREE_H
