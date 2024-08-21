#ifndef DICTIONARY_IMPL_H
#define DICTIONARY_IMPL_H

#include "Dictionary.h"
#include <iostream>

template <typename T>
bool LinkedList<T>::isEmpty() {
    return (head == nullptr);
}

template <typename T> 
T LinkedList<T>::remove(std::string name) {
    if (isEmpty()) {
        throw std::invalid_argument("object does not exist");
    }

    Node<T>* ptr = head;

    if (ptr->name == name) {
        head = nullptr;

        T temp = ptr->value;
        delete ptr;
        return temp;
    } 

    while (ptr->next != nullptr) {
        Node<T>* parent = ptr;
        ptr = ptr->next;

        if (ptr->name == name) {
            if (ptr->next == nullptr) {
                parent->next = nullptr;
            }
            else {
                parent->next = ptr->next;
            }

            T temp = ptr->value;
            delete ptr;
            return temp;
        }
    } 

    throw std::invalid_argument("object does not exist");
}

template <typename T> 
T& LinkedList<T>::add(std::string name, T value, bool& newEntry) {
    newEntry = true;

    if (isEmpty()) {
        head = new Node<T>(name, value);
        return head->value;
    }

    Node<T>* ptr = head;

    while (ptr->name != name && ptr->next != nullptr) { 
        ptr = ptr->next;
    }

    if (ptr->name == name) {
        newEntry = false;
        return ptr->value;  
    }

    ptr->next = new Node<T>(name, value);
    return ptr->next->value;
}

template <typename T>
T& LinkedList<T>::entry(std::string name) {
    if (isEmpty()) {
        throw std::invalid_argument("cannot get reference to a non-existant value");
    }

    Node<T>* ptr = head;
    if (ptr->name == name) {
        return ptr->value;
    }

    while (ptr->name != name && ptr->next != nullptr) { 
        ptr = ptr->next;
    }

    if(ptr->name == name) { 
        return ptr->value;
    }

    throw std::invalid_argument("cannot get reference to a non-existant value");
}

template <typename T>
bool LinkedList<T>::hasEntry(std::string name) {
    if (isEmpty()) {
        return false;
    }

    Node<T>* ptr = head;
    if (ptr->name == name) {
        return true;
    }

    while (ptr->name != name && ptr->next != nullptr) { 
        ptr = ptr->next;
    }

    return (ptr->name == name) ? true : false;
}


template <typename T>
int Dictionary<T>::hash(std::string name) {
    int value = 0;

    for (int i = 0; i < name.length(); i++) {
        value += name[i];
    }

    return value % length;
}

template <typename T>
T Dictionary<T>::remove(std::string name) {
    resetCache();
    return dict[hash(name)].remove(name);
}

template <typename T>
T& Dictionary<T>::add(std::string name, T initial) {
    bool newEntry;
    T& entry = dict[hash(name)].add(name, initial, newEntry);
    if (newEntry) {
        resetCache();
    }

    return entry;
}

template <typename T>
T& Dictionary<T>::entry(std::string name) {
    return dict[hash(name)].entry(name);
}

template <typename T>
void Dictionary<T>::updatePtrList() {
    _ptrList.clear();
    
    for (int i = 0; i < length; i++) {
        if (dict[i].isEmpty()) {
            continue;
        }

        Node<T>* ptr = dict[i].head;

        _ptrList.push_back(ptr);

        while (ptr->next != nullptr) {
            ptr = ptr->next;
            _ptrList.push_back(ptr);
        }
    }

    ptrListUpToDate = true;
}

template <typename T>
T& Dictionary<T>::getRef(int idx) { 
    if (!ptrListUpToDate) {
        updatePtrList();
    }

    return _ptrList[idx]->value;
}

template <typename T>
std::string Dictionary<T>::getID(int idx) {
    if (!ptrListUpToDate) {
        updatePtrList();
    }

    return _ptrList[idx]->name;
}

template <typename T>
int Dictionary<T>::size() {
    if (!ptrListUpToDate) {
        updatePtrList();
    }

    return _ptrList.size();
}

template <typename T>
bool Dictionary<T>::hasEntry(std::string name) {
    return dict[hash(name)].hasEntry(name);
}



#endif