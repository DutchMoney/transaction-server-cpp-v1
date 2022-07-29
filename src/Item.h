#include <string>


struct Item {
    Item(const std::string& name, int amount, float price ); 

    const std::string _name;
    const int _amount;
    const float _price;
};