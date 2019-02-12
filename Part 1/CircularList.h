

#ifndef LIST_STACK_H
#define LIST_STACK_H

#include <stdexcept>

using namespace std;

template<typename Elem>
class CircularList{
private:
    class CNode{
    private:
        Elem elem;
        CNode* next;
        friend class CircularList;
    public:
        CNode(): next(NULL){}
    };

public:
    CircularList();
    ~CircularList();
    bool empty() const;
    Elem& front() const;
    Elem& back() const;
    void advance();
    void add(const Elem& e);
    void remove();
    int size() const;
    void reverse();
    void cleanup();
private:
    CNode* cursor;
    CNode* avail;
    int n;
};

template<typename Elem>
CircularList<Elem>::CircularList(): cursor(NULL), avail(NULL), n(0){}

template<typename Elem>
CircularList<Elem>::~CircularList(){
    if(!empty()){
        CNode* newhead = cursor->next;
        cursor->next = avail;
        avail = newhead;
    }
    cleanup();
}

template<typename Elem>
inline bool CircularList<Elem>::empty() const{
    return n == 0;
}

template<typename Elem>
inline Elem& CircularList<Elem>::back() const{
        if(empty())
            throw runtime_error("can not return back of empty cir");
    return cursor->elem;
}

template<typename Elem>
inline Elem& CircularList<Elem>::front() const{
    if(empty())
        throw runtime_error("Can not return front of empty cir");
    return cursor->next->elem;
}

template<typename Elem>
inline void CircularList<Elem>::advance(){
    if(!empty())
        cursor = cursor->next;
}

template<typename Elem>
void CircularList<Elem>::add(const Elem& e){
    n++;
    if(!avail){
        avail = new CNode;
    }
    CNode* v = avail;
    avail = avail->next;
    v->elem = e;
    if(!cursor){
        v->next = v;
        cursor = v;
    }
    else{
        v->next = cursor->next;
        cursor->next = v;
    }
}

template<typename Elem>
void CircularList<Elem>::remove(){
    if(empty())
        throw runtime_error("can not remove from empty cirl");
    n--;
    CNode* old = cursor->next;
    if(old == cursor)
        cursor = NULL;
    else
        cursor->next = old->next;
    old->next = avail;
    avail = old;
}

template<typename Elem>
int CircularList<Elem>::size() const{
    return n;
}

template<typename Elem>
void CircularList<Elem>::reverse(){
    if(n == 2){
      cursor = cursor->next;
    }
    else if(n > 2){
        CNode* prev;
        CNode* cur;
        CNode* next;
        prev = cursor;
        cur = cursor->next;
        next = cur->next;
        cursor = cursor->next;
        for(int i = 0; i < n; i++){
            cur->next = prev;
            prev = cur;
            cur = next;
            next = next->next;
        }
    }
}

template<typename Elem>
void CircularList<Elem>::cleanup(){
    while(avail != NULL){
        CNode* old = avail;
        avail = avail->next;
        delete old;
    }
}


#endif //LIST_STACK_H
