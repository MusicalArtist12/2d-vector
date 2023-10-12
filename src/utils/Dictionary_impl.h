#include "utils/Dictionary.h"

#include <iostream>

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

template <typename T> 
T* linkedList<T>::pullItem(std::string id) {
    if(isEmpty()) return nullptr;

    node<T>* ptr = head;

    if(ptr->name == id) {
        if(ptr->next == nullptr) {
            head = nullptr;
        } else {
            head = ptr->next;
        }

        T* myValue = new T(ptr->value);
        delete ptr; 

        return myValue;
    } 

    if(ptr->next == nullptr) {
        return nullptr;
    }

    while(ptr->next != nullptr) {
        node<T>* tmp = ptr->next;

        if(tmp->name == id) {
            T* myValue = new T(tmp->value);
            ptr->next = tmp->next;
            delete tmp; 

            return myValue;
        }

        ptr = ptr->next;
    }

    return nullptr;
}

template <typename T> 
int dictionary<T>::hash(std::string str) {

    int hashValue = 26 * ((int)str[0] - 65); 

    if(str.length() > 1)
        hashValue += ((int)str[1] - 65); 

    hashValue = hashValue % length;

    return hashValue;
}

template <typename T>
int dictionary<T>::size() {
    int dictionarySize = 0;

    for(int i = 0; i < length; i++) {
        dictionarySize += dict[i].size();
    }

    return dictionarySize;
}

template <typename T>
bool dictionary<T>::hasItem(std::string name) {
    int pos = hash(name);

    node<T>* ptr = dict[pos].getItem(name); 

    if(ptr == nullptr) return false;

    return true;
    
}

template <typename T>
void dictionary<T>::addItem(T item, std::string name) {
    node<T>* ptr; 
    int count = 0;
    std::string finalName = name;

    do {
        ptr = dict[hash(finalName)].getItem(finalName);

        if(ptr != nullptr) {
            count++;
            finalName = name + " " + toString(count);
        } 
    } while (ptr != nullptr);

    if(finalName != name) {
        std::cout << "[dictionary]: Item \'" << name << "\' already exists, renaming to \'" << finalName << "\'"<< std::endl;
        
    }
    
    node<T>* n = new node<T>(item, finalName);

    dict[hash(finalName)].push(n);
}

template <typename T>
T& dictionary<T>::getItem(std::string name) {
    int pos = hash(name);

    node<T>* ptr = dict[pos].getItem(name);

    if(ptr == nullptr) {
        throw std::invalid_argument("item does not exist");
    }

    return ptr->value;
}

template <typename T>
T dictionary<T>::pullItem(std::string name) {
    int pos = hash(name);

    T* ptr = dict[pos].pullItem(name);

    if(ptr == nullptr) {
        throw std::invalid_argument("Item does not exist");
    }
    
    T myValue = *ptr;
    delete ptr;

    return myValue;
}

template <typename T>
std::string* dictionary<T>::getArrayOfIDs() {
    std::string* array = new std::string[size()];
    
    int idx = 0;
    
    for(int i = 0; i < length; i++) {
        if(dict[i].isEmpty()) continue;
        
        node<T>* ptr = dict[i].head;
        while(true) {
            array[idx] = ptr->name;

            idx++;
            if(ptr->next == nullptr) break;
            ptr = ptr->next;
        }
    }

    return array;
} 

