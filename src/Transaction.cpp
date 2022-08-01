#include "Transaction.h"
#include <functional>

Transaction::Transaction(const std::vector<Item>& items) {

    for (auto& item : items) {
       _itemMap.insert({item._name, {item._amount, item._price}}); 
    }
}

bool Transaction::isUserInTransaction(std::string userId) {
    auto userIt = _userMap.find(userId);

    if (userIt == _userMap.end()) return false;

    return true;
}

transaction_map::iterator Transaction::addUserIfNotExists(std::string userId) {
    auto userIt = _userMap.find(userId);

    if (userIt != _userMap.end()) return userIt;

    _userMap.insert({userId, {}});

    userIt = _userMap.find(userId);

    return userIt;
}

bool Transaction::removeUser(std::string userId) {
    if (!isUserInTransaction(std::ref(userId))) return false; 

    auto it = _userMap.find(userId);

    for (auto& itemPair : it->second) {
        const auto& [name, itemDescPair] = itemPair;
        const auto& [amount, price] = itemDescPair;

        updateItemMap<ADD>({name, amount, price});
    }

    _userMap.erase(userId);

    return true;
}

const transaction_map Transaction::getTransactionMap() {
    transaction_map tsMap{_userMap};

    tsMap.insert({"unused", _itemMap});

    return tsMap;
}

std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    transaction_map tsMap{t._userMap};
    tsMap.insert({"unused", t._itemMap});

    for (auto userIt : tsMap) {
        os << "-------------------------" << std::endl;
        os << "User: " << userIt.first << std::endl;
        os << "Items: ";
        int a = 1; 
        for (auto& itemIt : userIt.second) {
            auto& [amount, price] = itemIt.second;
            os << std::to_string(a++) + ". " << "name: " << itemIt.first << " amount: " << amount << " price: " << price << std::endl;
        }
        os << "-------------------------" << std::endl;
    }

    return os;
}
