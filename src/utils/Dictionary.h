#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>

template <typename T>
struct node {
    T value;
    const std::string name;
    node<T>* next;

    node(std::string& n, T val): value(val), name(n), next(nullptr) {}
};

template <typename T>
struct linkedList {
    node<T>* head = nullptr;

    bool isEmpty();

    T remove(std::string name);
    T& add(std::string name, T value, bool& newEntry);
    T& entry(std::string name);
};

template <typename T>
class dictionary {
    private:
        const int length;

        linkedList<T>* dict;
        int hash(std::string name);

        std::vector<node<T>*> _ptrList;
        bool ptrListUpToDate;

        void updatePtrList();

    public:
        dictionary(int n): length(n) {
            dict = new linkedList<T>[length];
            resetCache();
        };


        T remove(std::string name);   
        T& add(std::string name, T initial); 
        T& entry(std::string name);

        T& getRef(int idx);
        std::string getID(int idx);
        int size();
        

        inline void resetCache() {
            ptrListUpToDate = false;
        }

};

#endif