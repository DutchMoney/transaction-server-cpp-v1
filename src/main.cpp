#include "Transaction.h"
#include <vector>
#include <iostream>

int main() {
    Transaction t {std::vector<Item> {Item {"apple", 5, 5}}}; 

    std::cout << "asdasada" << std::endl;

    std::cout << t << std::endl;

    t.updateUserItem<UpdateType::ADD>("preman", {"apple", 2, 5});

    std::cout << t << std::endl;

    return 0;
}