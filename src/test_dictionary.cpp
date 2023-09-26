#include <iostream>
#include <string>
#include "dictionary.cpp"

int main() {
    dictionary<int> myDictionary;

    for(int i = 0; i < 26; i++ ) {
        std::string name1(1, (char)(65 + i));

        for(int j = 0; j < 26; j++) {
            std::string name2(1, (char)(65 + j));

            myDictionary.addItem(i, name1 + name2);
        }
       
    }
    
    std::cout << myDictionary.size() << std::endl;

    std::string* array = myDictionary.getArrayOfItems();


    for(int i = 0; i < myDictionary.size(); i++){
        std::cout << array[i] << ": " << myDictionary.getItem(array[i]) << std::endl;
    }

    
}