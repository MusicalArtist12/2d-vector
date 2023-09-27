#include "utils/Dictionary.h"

template <typename T> 
inline bool linkedList<T>::isEmpty() {
    if(head == nullptr) return true;
    else return false;
}

template <typename T> 
void linkedList<T>::push(node<T>* n) {
    if(isEmpty()) head = n;
    else {
        n->next = head;
        head = n;
    }
}

template <typename T> 
int linkedList<T>::size() {
    node<T>* n = head;
    int idx = 0;

    if(isEmpty()) return idx;

    while(n != nullptr) {
        idx++;
        n = n->next;
    }

    return idx;
}

template <typename T> 
node<T>* linkedList<T>::getItem(std::string id) {
    if(isEmpty()) return nullptr;

    node<T>* ptr = head;
    while(ptr->name != id) {
        
        if(ptr->next != nullptr) {
            ptr = ptr->next;
        } 

        else {
            return nullptr;
        }
    }

    return ptr;
}
