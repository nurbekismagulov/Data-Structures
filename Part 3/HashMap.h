

#ifndef ASSIGNMENT3_HASHMAP_H
#define ASSIGNMENT3_HASHMAP_H

// You are *not* allowed to include other header files

#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

using namespace std;

/*
 * This is an integer hash comparator that defines a hash function,
 * which is operator(). The hash function simply returns the integer that
 * is given as the input. See Page 387 of the textbook for more information.
 */
class IntHashComparator {
public:
  int operator()(const int& a) const {
    return a;
  }
};


/*
 * A Map based on hashing with separate chaining.
 * See Section 9.2.7 of the textbook for more detail.
 *
 * Notice that you can define any member variables.
 */
template <typename K, typename V, typename H>
class HashMap {
public:
  class Entry{
  public:
    Entry(const K& k = K(), const V& v = V()): _key(k), _value(v) {}
    const K& key() const { return _key; }
    V& value() { return _value; }
    void setKey(const K& k) { _key = k; }
    void setValue(const V& v) { _value = v; }
  private:
    const K _key;
    V _value;
  };
  class Iterator;
public:
  HashMap(int capacity = 100);
  int size() const;
  bool empty() const;
  Iterator find(const K& k);
  Iterator put(const K& k, const V& v);
  void erase(const K& k);
  void erase(const Iterator& p);
  Iterator begin();
  Iterator end();
  V& operator[](const K& k);
protected:
  typedef list<Entry> Bucket;
  typedef vector<Bucket> BktArray;
  typedef typename BktArray::iterator BItor;
  typedef typename Bucket::iterator EItor;
  Iterator finder(const K& k);
  Iterator inserter(const Iterator& p, const Entry& e);
  void eraser(const Iterator& p);
  static void nextEntry(Iterator& p){ ++p.ent; }
  static bool endOfBkt(const Iterator& p){
    return p.ent == p.bkt->end();
  }

private:
  int n;
  H hash;
  BktArray B;
public:
  class Iterator{
  private:
    EItor ent;
    BItor bkt;
    const BktArray* ba;
  public:
    Iterator(const BktArray& a, const BItor& b, const EItor& q = EItor()): ent(q), bkt(b), ba(&a) {}
    Entry& operator*() const{
      return *ent;
    }
    bool operator==(const Iterator& p) const{
      if(ba != p.ba || bkt != p.bkt)
        return false;
      else if(bkt == ba->end())
        return true;
      else
        return ent == p.ent;
    }
    Iterator& operator++(){
      ++ent;
      if(endOfBkt(*this)){
        ++bkt;
        while(bkt != ba->end() && bkt->empty())
          ++bkt;
        if(bkt == ba->end()) return *this;
        ent = bkt->begin();
      }
      return *this;
    }
    friend class HashMap;
  };
};

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator
HashMap<K,V,H>::end(){
  return Iterator(B, B.end());
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator
HashMap<K,V,H>::begin(){
  if(empty())
    return end();
  BItor bkt = B.begin();
  while(bkt->empty())
    ++bkt;
  return Iterator(B, bkt, bkt->begin());
}

template <typename K, typename V, typename H>
HashMap<K,V,H>::HashMap(int capacity): n(0), B(capacity) {}

template <typename K, typename V, typename H>
int HashMap<K,V,H>::size() const{ return n; }

template <typename K, typename V, typename H>
bool HashMap<K,V,H>::empty() const{ return size() == 0; }

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator
HashMap<K,V,H>::finder(const K& k){
  if(B.size() == 0)
    throw runtime_error("no divingding by zeto");
  int i = hash(k) % B.size();
  BItor bkt = B.begin() + i;
  Iterator p(B, bkt, bkt->begin());
  while(!endOfBkt(p) && (*p).key() != k)
    nextEntry(p);
  return p;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator
HashMap<K,V,H>::find(const K& k){
  Iterator p = finder(k);
  if(endOfBkt(p))
    return end();
  else
    return p;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator
HashMap<K,V,H>::inserter(const Iterator& p, const Entry& e){
  EItor ins = p.bkt->insert(p.ent, e);
  n++;
  return Iterator(B, p.bkt, ins);
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator
HashMap<K,V,H>::put(const K& k, const V& v){
  Iterator p = finder(k);
  if(endOfBkt(p))
    return inserter(p, Entry(k,v));
  else{
    p.ent->setValue(v);
    return p;
  }
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::eraser(const Iterator& p){
  p.bkt->erase(p.ent);
  n--;
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::erase(const Iterator& p){
  eraser(p);
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::erase(const K& k){
  Iterator p = finder(k);
  if(endOfBkt(p))
    throw runtime_error("Erase of nonexistent");
  eraser(p);
}

template <typename K, typename V, typename H>
V& HashMap<K,V,H>::operator[](const K& k){
  Iterator p = find(k);
  if(p == end()){
    V v;
    p = put(k,v);
  }
  return (*p).value();
}















#endif //ASSIGNMENT3_HASHMAP_H
