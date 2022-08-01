#include "Transaction.h"
#include <functional>
#include <iomanip>

Transaction::Transaction(const std::vector<Item>& items) {

    for (auto& item : items) {
       _itemMap.insert({item._name, {item._amount, item._price}}); 
    }
}

bool Transaction::isUserInTransaction(std::string userId) const {
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

const transaction_map Transaction::getTransactionMap() const {
    transaction_map tsMap{_userMap};

    tsMap.insert({"unused", _itemMap});

    return tsMap;
}

std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    auto& tsMap = t.getTransactionMap();

    os << "###################################" << std::endl;
    os << "| Transaction ID: " << 1 << std::endl;

    for (auto userIt : tsMap) {
        os << "|------------------------------" << std::endl;
        os << "| User: " << userIt.first << std::endl;
        os << std::left << std::setw(8) << std::setfill(' ') << "| Number";
        os << std::left << std::setw(8) << std::setfill(' ') << "| Name";
        os << std::left << std::setw(8) << std::setfill(' ') << "| Amount";
        os << std::left << std::setw(8) << std::setfill(' ') << "| Price" << std::endl;

        int a = 1; 
        for (auto& itemIt : userIt.second) {
            os << "|_______|_______|_______|_________" << std::endl;
            auto& [amount, price] = itemIt.second;
            os << std::left << std::setw(8) << std::setfill(' ') << "| "+std::to_string(a++);
            os << std::left << std::setw(8) << std::setfill(' ') << "| "+itemIt.first;
            os << std::left << std::setw(8) << std::setfill(' ') << "| "+std::to_string(amount);
            os << std::left << std::setw(8) << std::setfill(' ') << "| "+std::to_string(price) << std::endl;
        }
        os << "|------------------------------" << std::endl;
    }

    os << "###################################" << std::endl;

    return os;
}
