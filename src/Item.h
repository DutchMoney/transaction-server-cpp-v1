#ifndef ITEM_H
#define ITEM_H
#include <string>

struct Item {
    enum class ItemState {USED, IGNORED};
    Item(std::string_view name, int amount, float price, ItemState _name=ItemState::USED); 

    const std::string_view _name;
    const int _amount;
    const float _price;
    const ItemState _state;

};

#endif