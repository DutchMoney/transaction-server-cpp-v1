#include <vector>
#include "Item.h"
#include <unordered_map>
#include <iostream>
#include <string>

typedef std::unordered_map<std::string, std::tuple<int, float>> ItemMap;
typedef std::unordered_map<std::string, ItemMap> TransactionMap;

enum UpdateType {
    ADD,
    REMOVE
};

class Transaction {

public:
    Transaction(const std::vector<Item>& items);

    template <UpdateType T>
    bool updateUserItem(std::string userId, const Item& item);

    const TransactionMap getTransactionMap();

    friend std::ostream& operator<< (std::ostream& os, const Transaction& t);


// private:
    
    template <UpdateType T>
    bool isInItemMap(const Item& item);

    template <UpdateType T>
    bool isInUserMap(const Item& item, std::string userId);

    template <UpdateType T>
    bool updateItemMap(const Item& item);

    ItemMap _itemMap;
    TransactionMap _userMap;
};