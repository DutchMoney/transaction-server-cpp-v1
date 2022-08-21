#include "Item.h"

Item::Item(std::string_view name, int amount, float price) : _name(name) , _amount(amount), _price(price) {}