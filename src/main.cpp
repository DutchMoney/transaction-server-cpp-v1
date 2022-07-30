#include "Transaction.h"
#include <vector>
#include <iostream>

int main() {
    Transaction t {std::vector<Item> {Item {"apple", 5, 5}}}; 

    std::cout << "asdasada" << std::endl;
    
    for (auto& it : t._itemMap)

    std::cout << t << std::endl;


    return 0;
}