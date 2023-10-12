#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>

template <typename T>
struct node {
    T value;
    std::string name;
    node<T>* next;

    node(std::string& n, T val): value(val), name(n), next(nullptr) {}
};

template <typename T>
struct linkedList {
    node<T>* head = nullptr;

    bool isEmpty();
    int size();

    T remove(std::string name);
    T& add(std::string name, T value);
    T& entry(std::string name);
};

template <typename T>
class dictionary {
    private:
        const int length;

        linkedList<T>* dict;
        int hash(std::string name);

    public:
        dictionary(int n): length(n) {dict = new linkedList<T>[length];};

        int size();

        T remove(std::string name);   
        T& add(std::string name, T value); 
        T& entry(std::string name);
        std::vector<std::string> nameList();
};

#endif