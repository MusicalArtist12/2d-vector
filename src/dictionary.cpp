#pragma once

#include <string>
#include "Utils.cpp"

#define DICTIONARY_SIZE 676

template <typename T>
class dictionary {
    public:
        struct node {
            T value;
            std::string name;
            node* next = nullptr;
        };

        struct llist {
            node* head = nullptr;

            bool isEmpty() {
                if(head == nullptr) return true;

                else return false;
            }

            void push(node* n) {
                if(isEmpty()) head = n;
                else {
                    n->next = head;
                    head = n;
                }
            }

            /*
            T operator[](int n) {
                node *ptr = head;

                for(int i = 0; i < n; i++) {
                    if(ptr->next == nullptr) 
                        throw std::invalid_argument("Dictionary out of bounds");

                    ptr = ptr->next;
                }

                return *ptr;
            }
            */

            int size() {
                node* n = head;
                int idx = 0;

                if(isEmpty()) return idx;

                while(n != nullptr) {
                    idx++;
                    n = n->next;
                }

                return idx;
            }
        
            node* getItem(std::string id) {
                if(isEmpty()) return nullptr;

                node* ptr = head;
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
        };

        int hash(std::string str) {
            /* alphabetize based off first char
                #define DICTIONARY_SIZE 26
                int hashValue = 0;
                hashValue = (int)str[0] - 65; 
            */

            /* alphabetize based off first two char's
                #define DICTIONARY_SIZE 1000
            */

            int hashValue = 26 * ((int)str[0] - 65); 

            if(str.length() > 1)
                hashValue += ((int)str[1] - 65); 
        
            hashValue = hashValue % DICTIONARY_SIZE;

            return hashValue;
        }


        void addItem(T item, std::string name) {
            node* ptr; 
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
            
            node *n = new node;
            n->name = finalName;
            n->value = item;

            dict[hash(finalName)].push(n);
        }

        T& getItem(std::string name) {
            int pos = hash(name);
            int lsize = dict[pos].size();

            node* ptr = dict[pos].getItem(name);

            if(ptr == nullptr) {
                std::cout << "[dictionary]: Item \'" << name << "\' does not exist" << std::endl;
                throw std::invalid_argument("item does not exist");
            }
        
            return ptr->value;
        }



        int size() {
            int dictionarySize = 0;

            for(int i = 0; i < DICTIONARY_SIZE; i++) {
                dictionarySize += dict[i].size();
            }

            return dictionarySize;
        }

        std::string* getArrayOfItems() {
            std::string* array = new std::string[size()];
            
            int idx = 0;
            
            for(int i = 0; i < DICTIONARY_SIZE; i++) {
                if(dict[i].isEmpty()) continue;
                
                node* ptr = dict[i].head;
                while(true) {
                    array[idx] = ptr->name;

                    idx++;
                    if(ptr->next == nullptr) break;
                    ptr = ptr->next;
                }
            }

            return array;
        } 

    private:
        llist dict[DICTIONARY_SIZE];
        

};