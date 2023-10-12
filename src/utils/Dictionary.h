#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "utils/Utils.h"

#include <string>


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
    T* pullItem(std::string id);
};

template <typename T>
class dictionary {
    private:
        const int length;

        linkedList<T>* dict;
        int hash(std::string str);

    public:
        int size();

        bool hasItem(std::string name);

        void addItem(T item, std::string name);
        T& getItem(std::string name);
        T pullItem(std::string name);
        
        std::string* getArrayOfIDs();

        dictionary(int len): length(len) {
            dict = new linkedList<T>[length];
        }
};

#endif