#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <vector>
#include "Item.h"
#include <unordered_map>
#include <iostream>
#include <string>

typedef std::unordered_map<std::string, std::tuple<int, float>> item_map;
typedef std::unordered_map<std::string, item_map> transaction_map;


class Transaction {

public:
    Transaction(const std::vector<Item>& items);

    enum UpdateType {
        ADD,
        REMOVE
    };

    bool isUserInTransaction(std::string userId);

    transaction_map::iterator addUserIfNotExists(std::string userId);

    bool removeUser(std::string userId);

    const transaction_map getTransactionMap();

    friend std::ostream& operator<< (std::ostream& os, const Transaction& t);

    template <UpdateType T>
    bool updateUserItem(std::string userId, const Item& item) {
        if (!isUserInTransaction(userId)) return false;

        if (!isItemUnused<T == ADD ? REMOVE : ADD>(item)) return false;

        if (!updateItemMap<T == ADD ? REMOVE : ADD>({item._name, item._amount, item._price})) return false;

        auto userIt = _userMap.find(userId);

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

private:
    
    template <UpdateType T>
    bool isItemUnused(const Item& item) {

        auto it = _itemMap.find(item._name);

        if (it == _itemMap.end()) return false;

        if (T == UpdateType::ADD) return true;

        auto [amount, price] = it->second;

        if (amount < item._amount || price != item._price) return false;

        return true;
    }

    template <UpdateType T>
    bool isUserHasItem(const Item& item, std::string userId) {

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
    bool updateItemMap(const Item& item) {

        if (!isItemUnused<T>(item)) {
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

    item_map _itemMap;
    transaction_map _userMap;
};

#endif