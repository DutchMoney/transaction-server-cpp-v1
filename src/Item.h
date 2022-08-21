#ifndef ITEM_H
#define ITEM_H
#include <string>

struct Item {
    Item(std::string_view name, int amount, float price ); 

    const std::string_view _name;
    const int _amount;
    const float _price;
};

#endif