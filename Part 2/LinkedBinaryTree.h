
// LinkedBinaryTree.h (Version 1.0)
//
// Please write your name, your student ID, and your email here.
// Moreover, please describe the implementation of your functions here.
// You will have to submit this file.
//
/*
    Daniyar Abesbek 20162001 daniyarabesbek@gmail.com
    All functions implemented based on textbook and lecture slides without bugs
*/

#ifndef kkk
#define kkk

#include<list>
#include<stdexcept>

using namespace std;

template<typename Elem>
class LinkedBinaryTree{
protected:
  struct Node{
    Elem elt;
    Node* par;
    Node* left;
    Node* right;
    Node(): par(NULL), left(NULL), right(NULL) {}
  };
public:
  class Position{
  public:
    Position(Node* _v = NULL): v(_v){}
    Elem& operator*() { return v-> elt; }
    Position left() const { if(v == NULL) throw runtime_error("bad pos"); return Position(v->left); }
    Position right() const { if(v == NULL) throw runtime_error("bad pos"); return Position(v->right); }
    Position parent() const { if(v == NULL) throw runtime_error("bad pos"); return Position(v->par); }
    bool isRoot() const { if(v == NULL) throw runtime_error("bad pos"); return v->par == NULL; }
    bool isExternal() const { if(v == NULL) throw runtime_error("bad pos"); return v->left == NULL && v->right == NULL; }
    bool isNull() const { return v == NULL; }
    Elem& operator*() const { return v->elt; }
    friend class LinkedBinaryTree;
  private:
    Node* v;
  };
public:
  typedef list<Position> PositionList;
  LinkedBinaryTree();
  ~LinkedBinaryTree();
  int size() const;
  bool empty() const;
  Position root() const;
  PositionList positions() const;
  void addRoot();
  void expandExternal(const Position& p);
  Position removeAboveExternal(const Position& p);
  int height() const;
  void attachLeftSubtree(const Position& p, LinkedBinaryTree& subtree);
  void attachRightSubtree(const Position& p, LinkedBinaryTree& subtree);
  void removeSubtree(const Position& p);
protected:
  void preorder(Node* v, PositionList& pl) const;
private:
  int deep(const Position& p) const;
  void removeSubtreewithoutpar(const Position& p);
private:
  Node* _root;
  int n;
};

template<typename Elem>
LinkedBinaryTree<Elem>::LinkedBinaryTree(): _root(NULL), n(0) {}

template<typename Elem>
LinkedBinaryTree<Elem>::~LinkedBinaryTree(){
}

template<typename Elem>
int LinkedBinaryTree<Elem>::size() const{
  return n;
}

template<typename Elem>
bool LinkedBinaryTree<Elem>::empty() const{
  return n == 0;
}

template<typename Elem>
typename LinkedBinaryTree<Elem>::Position
LinkedBinaryTree<Elem>::root() const {
  return Position(_root);
}

template<typename Elem>
void LinkedBinaryTree<Elem>::addRoot(){
  if(!empty())
    throw runtime_error("Can not add root to empty tree");
  _root = new Node;
  n = 1;
}

template<typename Elem>
void LinkedBinaryTree<Elem>::expandExternal(const Position& p){
  if(p.isNull())
    throw runtime_error("cant expand null");
  if(!(p.isExternal()) && !(p.isNull()) )
    throw runtime_error("cant expand not external node");
  Node*v = p.v;
  v->left = new Node;
  v->left->par = v;
  v->right = new Node;
  v->right->par = v;
  n += 2;
}

template<typename Elem>
typename LinkedBinaryTree<Elem>::Position
LinkedBinaryTree<Elem>::removeAboveExternal(const Position& p){
  if(p.isNull())
    throw runtime_error("cant delete null");
  if(!p.isExternal())
    throw runtime_error("cant removeaboveexternal internal node");
  Node* w = p.v;
  if(p.isRoot()){
    _root = NULL;
    delete w;
    n = 0;
    return Position(NULL);
  }
  else{
    Node* v = w->par;
    Node* sib;
    if(w == v->left)
      sib = v->right;
    else
      sib = v->right;
    if(v == _root){
      _root = sib;
      sib->par = NULL;
    }
    else {
      Node* gpar = v->par;
      if(v == gpar->left)
        gpar->left = sib;
      else
      gpar->right = sib;
      sib->par = gpar;
    }
    delete w;
    delete v;
    n -= 2;
    return Position(sib);
  }
}

template<typename Elem>
int LinkedBinaryTree<Elem>::deep(const Position& p) const{
  int cnt = 0;
  Position currentpos = p;
  while(!currentpos.isRoot()){
    cnt++;
    currentpos = currentpos.parent();
  }
  return cnt;
}

template<typename Elem>
int LinkedBinaryTree<Elem>::height() const{
  if(empty())
    throw runtime_error("tree has no height");
  int maxheight = 0;
  PositionList pl = positions();
  for(typename PositionList::iterator it = pl.begin(); it != pl.end(); ++it){
    Position pos = *it;
    int deeps = deep(pos);
    maxheight = max(maxheight, deeps);
  }
  return maxheight;
}

template<typename Elem>
void LinkedBinaryTree<Elem>::removeSubtreewithoutpar(const Position& p){
  if(p.v->left)
    removeSubtreewithoutpar(p.left());
  if(p.v->right)
    removeSubtreewithoutpar(p.right());
  delete p.v;
  n--;
}

template<typename Elem>
void LinkedBinaryTree<Elem>::attachLeftSubtree(const Position& p, LinkedBinaryTree& subtree){
  if(p.isNull())
    throw runtime_error("cant attach to null");
  if(!p.left().isNull())
    throw runtime_error("cant attach to existing left child");
  if(subtree.root().isNull())
    throw runtime_error("cant attach empty sbtree");
  if(!p.v->left){
      p.v->left = subtree._root;
      subtree._root->par = p.v;
  }
  subtree._root = NULL;
  n += subtree.n;
  subtree.n = 0;
}

template<typename Elem>
void LinkedBinaryTree<Elem>::attachRightSubtree(const Position& p, LinkedBinaryTree& subtree){
  if(p.isNull())
    throw runtime_error("cant attach to null");
  if(!p.right().isNull())
    throw runtime_error("cant attach to existing righft child");
  if(subtree.root().isNull())
    throw runtime_error("cant attach empty sbtree");
  if(!p.v->right){
      p.v->right = subtree._root;
      subtree._root->par = p.v;
  }
  subtree._root = NULL;
  n += subtree.n;
  subtree.n = 0;
}

template<typename Elem>
void LinkedBinaryTree<Elem>::removeSubtree(const Position& p){
  if(p.isNull())
    throw runtime_error("cant remove empty subtree");
  if(p.v->par){
    if(p.v == p.v->par->left)
      p.v->par->left = NULL;
    else
      p.v->par->right = NULL;
  }
  removeSubtreewithoutpar(p);
}

template<typename Elem>
typename LinkedBinaryTree<Elem>::PositionList
LinkedBinaryTree<Elem>::positions() const{
  PositionList pl;
  preorder(_root, pl);
  return PositionList(pl);
}


template<typename Elem>
void LinkedBinaryTree<Elem>::preorder(Node* v, PositionList& pl) const{
  if(v == NULL)
    return;
  pl.push_back(Position(v));
  if(v->left)
    preorder(v->left, pl);
  if(v->right)
    preorder(v->right, pl);
}

#endif
