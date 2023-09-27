#include "utils/Dictionary.h"

template <typename T> 
int dictionary<T>::hash(std::string str) {

    int hashValue = 26 * ((int)str[0] - 65); 

    if(str.length() > 1)
        hashValue += ((int)str[1] - 65); 

    hashValue = hashValue % DICTIONARY_SIZE;

    return hashValue;
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
        std::cout << "[dictionary]: Item \'" << name << "\' does not exist" << std::endl;
        throw std::invalid_argument("item does not exist");
    }

    return ptr->value;
}

template <typename T>
int dictionary<T>::size() {
    int dictionarySize = 0;

    for(int i = 0; i < DICTIONARY_SIZE; i++) {
        dictionarySize += dict[i].size();
    }

    return dictionarySize;
}

template <typename T>
std::string* dictionary<T>::getArrayOfItems() {
    std::string* array = new std::string[size()];
    
    int idx = 0;
    
    for(int i = 0; i < DICTIONARY_SIZE; i++) {
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

