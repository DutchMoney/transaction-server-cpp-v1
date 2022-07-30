#include "Transaction.h"
#include <functional>

Transaction::Transaction(const std::vector<Item>& items) {

    for (auto& item : items) {
       _itemMap.insert({item._name, {item._amount, item._price}}); 
    }
}

const TransactionMap Transaction::getTransactionMap() {
    TransactionMap tsMap{_userMap};

    tsMap.insert({"unused", _itemMap});

    return tsMap;
}

std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    TransactionMap tsMap{t._userMap};
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

template <UpdateType T>
bool Transaction::updateUserItem(std::string userId, const Item& item) {
    if (!isInItemMap<T == ADD ? REMOVE : ADD>(item)) return false;

    if (!updateItemMap<T == ADD ? REMOVE : ADD>({item._name, item._amount, item._price})) return false;

    auto userIt = _userMap.find(userId);
    if (userIt == _userMap.end()) {
        if (T == UpdateType::REMOVE) return false;

        _userMap.insert({userId, {}});

        return true;
    }

    auto itemIt = userIt->second.find(item._name);
    if (itemIt == userIt->second.end()) {
        if (T == UpdateType::REMOVE) return false;
        
        userIt->second.insert({item._name, {item._amount, item._price}});

        return true;
    }

    auto& [amount, price] = itemIt->second;
    const int newAmount = T == UpdateType::ADD ? amount + item._amount : amount - item._amount;

    if (newAmount == 0) userIt->second.erase(item._name);
    else if (newAmount < 0) return false;
    else amount = newAmount;

    return true;
}

template <UpdateType T>
bool Transaction::isInItemMap(const Item& item) {
    auto it = _itemMap.find(item._name);

    if (it == _itemMap.end()) return false;

    if (T == UpdateType::ADD) return true;

    auto [amount, price] = it->second;

    if (amount < item._amount || price != item._price) return false;

    return true;
}

template <UpdateType T>
bool Transaction::isInUserMap(const Item& item, std::string userId) {
    auto userIt = _userMap.find(userId);
    if (userIt == _userMap.end()) return false;

    auto itemIt = userIt->second.find(item._name);
    if (itemIt == userIt->second.end()) return false;

    if (T == UpdateType::ADD) return true;

    auto [amount, price] = itemIt->second;
    if (item._amount > amount || item._price != price) return false;

    return true;
}

template <UpdateType T>
bool Transaction::updateItemMap(const Item& item) {
    if (!isInItemMap<T>(item)) {
        if (T == UpdateType::ADD) {
            _itemMap.insert({item._name, {item._amount, item._price}});

            return true;
        }

        return false;
    }

    auto& [amount, price] = _itemMap.find(item._name)->second;
    
    const int newAmount = T == UpdateType::ADD ? amount + item._amount : amount - item._amount;

    if (newAmount == 0) _itemMap.erase(item._name);
    else if (newAmount < 0) return false;
    else amount = newAmount;

    return true;
}