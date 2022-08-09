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

bool Transaction::addUserIfNotExists(std::string userId) {
    auto userIt = _userMap.find(userId);

    if (userIt != _userMap.end()) return false;

    _userMap.insert({userId, {}});

    userIt = _userMap.find(userId);

    return true;
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

bool Transaction::updateItemPrice(std::string name, float price) {
    if (price < 0) return false;

    if (isItemUnused<ADD>({name, 0, price})) {
        auto itemIt = _itemMap.find(name);

        auto& [_amount, _price] = itemIt->second;

        _price = price;
        
        for (auto& userIt : _userMap) {
            if (!isUserHasItem<ADD>({name, 0, price}, userIt.first)) continue;

            auto itemIt = userIt.second.find(name);
            auto& [_amount, _price] = itemIt->second;

            _price = price;
        }

        return true;
    }

    return false;
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

template <>
bool Transaction::updateUserItem<Transaction::UpdateType::ADD>(std::string userId, const Item& item) {

    if (!isUserInTransaction(userId)) return false;

    if (!updateItemMap<REMOVE>({item._name, item._amount, item._price})) return false;

    auto userIt = _userMap.find(userId);

    auto itemIt = userIt->second.find(item._name);
    if (itemIt == userIt->second.end()) {
        userIt->second.insert({item._name, {item._amount, item._price}});

        return true;
    }

    auto& [amount, price] = itemIt->second;
    const int newAmount =  amount + item._amount; 

    if (newAmount == 0) userIt->second.erase(item._name);
    else if (newAmount < 0) return false;
    else amount = newAmount;

    return true;
}

template <>
bool Transaction::updateUserItem<Transaction::UpdateType::REMOVE>(std::string userId, const Item& item) {

    if (!isUserInTransaction(userId)) return false;

    auto userIt = _userMap.find(userId);

    auto itemIt = userIt->second.find(item._name);
    if (itemIt == userIt->second.end()) return false;

    auto& [amount, price] = itemIt->second;
    const int newAmount = amount - item._amount;

    if (newAmount == 0) userIt->second.erase(item._name);
    else if (newAmount < 0) return false;
    else amount = newAmount;

    if (!updateItemMap<ADD>({item._name, item._amount, item._price})) return false;


    return true;
}

