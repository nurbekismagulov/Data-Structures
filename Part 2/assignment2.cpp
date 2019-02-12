

#include <iostream>
#include <stdexcept>
#include <list>
#include <stack>
#include <limits>
#include "LinkedBinaryTree.h"
#include "Symbol.h"
#include "assignment2.h"


LinkedBinaryTree<Symbol> parsePostfixExpression(list<string> tokens) {
    stack<LinkedBinaryTree<Symbol> > tempstack;
    Symbol tempsymbol;
    for(list<string>::iterator it = tokens.begin(); it != tokens.end(); ++it){
        tempsymbol = *it;
        if(!tempsymbol.isOperator()){
            LinkedBinaryTree<Symbol> temptree;
            temptree.addRoot();
            *(temptree.root()) = tempsymbol;
            tempstack.push(temptree);
        }
        else{
            if(tempstack.size() < 2)
              throw runtime_error("incorrect expression");
            LinkedBinaryTree<Symbol> left, right;
            right = tempstack.top();
            tempstack.pop();
            left = tempstack.top();
            tempstack.pop();
            LinkedBinaryTree<Symbol> ans;
            ans.addRoot();
            *(ans.root()) = tempsymbol;
            ans.attachLeftSubtree(ans.root(), left);
            ans.attachRightSubtree(ans.root(), right);
            tempstack.push(ans);
        }
    }
    if(tempstack.size() != 1)
      throw runtime_error("bad postfix");
    return tempstack.top();
}

void recursive_inorder(LinkedBinaryTree<Symbol>::Position pos){
    if(pos.isExternal()){
        cout << *pos;
        return;
    }
    cout << "( ";
    recursive_inorder(pos.left());
    cout << " ";
    cout << *pos;
    cout << " ";
    recursive_inorder(pos.right());
    cout << " )";
}


void print_inorder(const LinkedBinaryTree<Symbol>& tree) {
    recursive_inorder(tree.root());
}

void recursive_postorder(LinkedBinaryTree<Symbol>::Position pos){
    if(pos.isExternal()){
        cout << *pos << " ";
        return;
    }
    recursive_postorder(pos.left());
    recursive_postorder(pos.right());
    cout << *pos << " ";
}

void print_postorder(const LinkedBinaryTree<Symbol>& tree) {
    recursive_postorder(tree.root());
}

int findMinimumDepth(const LinkedBinaryTree<Symbol>& tree, const Symbol& sym) {
    LinkedBinaryTree<Symbol>::PositionList pl = tree.positions();
    Symbol tempsymbol;
    LinkedBinaryTree<Symbol>::Position pos;
    int mindepth = 1000;
    bool status = 0;
    for(typename LinkedBinaryTree<Symbol>::PositionList::iterator it = pl.begin(); it != pl.end(); ++it){
        pos = *it;
        tempsymbol = *pos;
        int current_depth = 0;
        if(tempsymbol == sym){
            status = 1;
            while(!pos.isRoot()){
                current_depth++;
                pos = pos.parent();
            }
            if(mindepth > current_depth)
                mindepth = current_depth;
        }
    }
    if(status)
      return mindepth;
    else
      return -1;
}

void substitute(const LinkedBinaryTree<Symbol>& tree, const Symbol& variable, const Symbol& constant) {
  LinkedBinaryTree<Symbol>::PositionList pl = tree.positions();
  Symbol tempsymbol;
  for(typename LinkedBinaryTree<Symbol>::PositionList::iterator it = pl.begin(); it != pl.end(); ++it){
      tempsymbol = **it;
      if(tempsymbol == variable){
          **it = constant;
      }
  }
}


void simplify_subtree(LinkedBinaryTree<Symbol>& tree) {
  LinkedBinaryTree<Symbol>::PositionList pl = tree.positions();
  Symbol tempsymbol;
  LinkedBinaryTree<Symbol>::Position pos;
  for(LinkedBinaryTree<Symbol>::PositionList::reverse_iterator it = pl.rbegin(); it != pl.rend(); ++it){
    pos = *it;
    tempsymbol = *pos;
    if(tempsymbol.isOperator() && (*pos.left()).isConstant() && (*pos.right()).isConstant()){
      int left_constant = (*pos.left()).getConstant();
      int right_constant = (*pos.right()).getConstant();
      int ans = tempsymbol.compute(left_constant, right_constant);
      tree.removeSubtree(pos.left());
      tree.removeSubtree(pos.right());
      **it = Symbol(ans);
    }
  }
}
