#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "utils/Utils.h"

#include <string>
#include <iostream>

#define DICTIONARY_SIZE 676

template <typename T>
struct node {
    T value;
    std::string name;
    node<T>* next;

    node(T& val, std::string& n): value(val), name(n), next(nullptr) {}
};

template <typename T>
struct linkedList {
    node<T>* head = nullptr;

    inline bool isEmpty();
    void push(node<T>* n);
    int size();

    node<T>* getItem(std::string id);
};

template <typename T>
class dictionary {
    private:
        linkedList<T> dict[DICTIONARY_SIZE];
        int hash(std::string str);

    public:
        void addItem(T item, std::string name);
        T& getItem(std::string name);
        
        int size();
        std::string* getArrayOfItems();
};

#endif