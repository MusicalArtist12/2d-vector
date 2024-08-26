#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>

template <typename T>
struct Node {
    T value;
    const std::string name;
    Node<T>* next;

    Node(std::string& n, T val): value(val), name(n), next(nullptr) {}
};

template <typename T>
struct LinkedList {
    Node<T>* head = nullptr;

    bool isEmpty();

    T remove(std::string name);
    T& add(std::string name, T value, bool& newEntry);
    T& entry(std::string name);
    bool hasEntry(std::string name);
};

template <typename T>
class Dictionary {
    private:
        const int length;

        LinkedList<T>* dict;
        int hash(std::string name);

        std::vector<Node<T>*> _ptrList;
        bool ptrListUpToDate;

        void updatePtrList();

    public:
        Dictionary(int n): length(n) {
            dict = new LinkedList<T>[length];
            resetCache();
        };


        T remove(std::string name);   

        // does not change the value if it already exists
        T& add(std::string name, T initial); 
        T& entry(std::string name);

        T& getRef(int idx);
        bool hasEntry(std::string name);
        std::string getID(int idx);
        int size();
        

        inline void resetCache() {
            ptrListUpToDate = false;
        }

};

#endif