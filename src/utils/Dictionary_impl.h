#ifndef DICTIONARY_IMPL_H
#define DICTIONARY_IMPL_H

#include "Dictionary.h"

#include <iostream>

template <typename T>
bool linkedList<T>::isEmpty() {
    return (head == nullptr);
}

template <typename T> 
T linkedList<T>::remove(std::string name) {
    if(isEmpty()) throw std::invalid_argument("object does not exist");

    node<T>* ptr = head;

    if(ptr->name == name) {
        head = nullptr;

        T temp = ptr->value;
        delete ptr;
        return temp;

    } 

    while(ptr->next != nullptr) {
        node<T>* parent = ptr;
        ptr = ptr->next;

        if(ptr->name == name) {
            if(ptr->next == nullptr) parent->next = nullptr;
            else parent->next = ptr->next;

            T temp = ptr->value;
            delete ptr;
            return temp;
        }
    } 

    throw std::invalid_argument("object does not exist");
}

template <typename T> 
T& linkedList<T>::add(std::string name, T value, bool& newEntry) {
    newEntry = true;

    if(isEmpty()) {
        head = new node<T>(name, value);
        return head->value;
    }

    node<T>* ptr = head;

    while(ptr->name != name && ptr->next != nullptr) { 
        ptr = ptr->next;
    }

    if(ptr->name == name) {
        newEntry = false;
        return ptr->value;  
    }

    
    ptr->next = new node<T>(name, value);
    return ptr->next->value;
}

template <typename T>
T& linkedList<T>::entry(std::string name) {
    if(isEmpty()) throw std::invalid_argument("cannot get reference to a non-existant value");

    node<T>* ptr = head;
    if(ptr->name == name) return ptr->value;

    while(ptr->name != name && ptr->next != nullptr) { 
        ptr = ptr->next;
    }

    if(ptr->name == name) return ptr->value;

     throw std::invalid_argument("cannot get reference to a non-existant value");
}

template <typename T>
int dictionary<T>::hash(std::string name) {
    int value = 0;

    for(int i = 0; i < name.length(); i++) {
        value += name[i];
    }

    return value % length;
}

template <typename T>
T dictionary<T>::remove(std::string name) {
    resetCache();
    return dict[hash(name)].remove(name);
}

template <typename T>
T& dictionary<T>::add(std::string name, T value) {
    
    bool newEntry;
    T& entry = dict[hash(name)].add(name, value, newEntry);
    if(newEntry) resetCache();

    return entry;
}

template <typename T>
T& dictionary<T>::entry(std::string name) {
    return dict[hash(name)].entry(name);
}

template <typename T>
void dictionary<T>::updatePtrList() {
    _ptrList.clear();
    
    for(int i = 0; i < length; i++) {
        if(dict[i].isEmpty()) continue;

        node<T>* ptr = dict[i].head;

        _ptrList.push_back(ptr);

        while(ptr->next != nullptr) {
            ptr = ptr->next;
            _ptrList.push_back(ptr);
        }
    }

    ptrListUpToDate = true;
}

template <typename T>
T& dictionary<T>::getRef(int idx) { 
    if(!ptrListUpToDate) updatePtrList();

    return _ptrList[idx]->value;
}

template <typename T>
std::string dictionary<T>::getID(int idx) {
    if(!ptrListUpToDate) updatePtrList();

    return _ptrList[idx]->name;
}

template <typename T>
int dictionary<T>::size() {
    if(!ptrListUpToDate) updatePtrList();

    return _ptrList.size();
}


#endif