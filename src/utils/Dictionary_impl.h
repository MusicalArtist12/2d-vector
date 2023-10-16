#ifndef DICTIONARY_IMPL_H
#define DICTIONARY_IMPL_H

#include "Dictionary.h"

#include <iostream>

template <typename T>
bool linkedList<T>::isEmpty() {
    return (head == nullptr);
}

template <typename T>
int linkedList<T>::size() {
    if(isEmpty()) return 0;

    int count = 1;
    node<T>* ptr = head;

    while(ptr->next != nullptr) {
        count++;
        ptr = ptr->next;
    }

    return count;
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
T& linkedList<T>::add(std::string name, T value) {
    if(isEmpty()) {
        head = new node<T>(name, value);
        return head->value;
    }

    node<T>* ptr = head;

    while(ptr->name != name && ptr->next != nullptr) { 
        ptr = ptr->next;
    }

    if(ptr->name == name) return ptr->value;

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
int dictionary<T>::size() {
    int len = 0;
    for(int i = 0; i < length; i++) {
        if(!dict[i].isEmpty()) len += dict[i].size();
    }

    return len;
}

template <typename T>
T dictionary<T>::remove(std::string name) {
    return dict[hash(name)].remove(name);
}

template <typename T>
T& dictionary<T>::add(std::string name, T value) {
    return dict[hash(name)].add(name, value);
}

template <typename T>
T& dictionary<T>::entry(std::string name) {
    return dict[hash(name)].entry(name);
}

template <typename T>
std::vector<std::string> dictionary<T>::nameList() {
    std::vector<std::string> names;

    for(int i = 0; i < length; i++) {
        if(dict[i].isEmpty()) continue;

        node<T>* ptr = dict[i].head;
        names.push_back(ptr->name);

        while(ptr->next != nullptr) {
            ptr = ptr->next;
            names.push_back(ptr->name);
        }
    }

    return names;
}

#endif