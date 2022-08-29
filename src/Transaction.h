#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <vector>
#include "Item.h"
#include <unordered_map>
#include <iostream>
#include <string>
#include <shared_mutex>
#include <mutex>
#include <thread>

typedef std::unordered_map<std::string_view, std::tuple<int, float>> item_map;
typedef std::unordered_map<std::string_view, item_map> transaction_map;


class Transaction {

public:
    Transaction(const std::string_view& tId); //Empty Transaction
    Transaction(const std::string_view& tId, const std::vector<Item>& items); //Filled Transaction

    Transaction(const Transaction& t);

    enum class UpdateType {
        ADD,
        REMOVE
    };

    bool isUserInTransaction(std::string_view userId) const;

    bool addUser(std::string_view userId);

    bool removeUser(std::string_view userId);

    const transaction_map getTransactionMap() const;

    bool updateItemPrice(std::string_view name, float price);

    friend std::ostream& operator<< (std::ostream& os, const Transaction& t);

    template <UpdateType T>
    bool updateUserItem(const std::string_view& userId, const Item& item);

    constexpr std::string_view getTid() {return _tId;} 
private:


    template <UpdateType T>
    bool isUserHasItem(const Item& item, std::string_view userId) const {

        if(!isUserInTransaction(userId)) return false;

        auto userIt = _userMap.find(userId);

        auto itemIt = userIt->second.find(item._name);
        if (itemIt == userIt->second.end()) return false;

        if (T == UpdateType::ADD) return true;

        auto [amount, price] = itemIt->second;
        if (item._amount > amount || item._price != price) return false;

        return true;
    }

    template <UpdateType T>
    bool isItemUnused(const Item& item) const {
        
        std::shared_lock<std::shared_mutex> readLock{transactionItemMutex};

        if (item._amount < 0) return false;

        auto it = _itemMap.find(item._name);
        if (it == _itemMap.end()) return false;

        if (T == UpdateType::ADD) return true;

        auto [amount, price] = it->second;
        if (amount < item._amount || price != item._price) return false;

        return true;
    } 



    template <UpdateType T>
    bool updateItemMap(const Item& item) {

        if (!isItemUnused<T>(item)) {
            if (T == UpdateType::REMOVE) return false;
            
            std::scoped_lock<std::shared_mutex> writeLock{transactionItemMutex};
            
            _itemMap.insert({item._name, {item._amount, item._price}});
            return true;
        }

        std::scoped_lock<std::shared_mutex> writeLock{transactionItemMutex};

        auto& [amount, price] = _itemMap.find(item._name)->second;
        const int newAmount = T == UpdateType::ADD ? amount + item._amount : amount - item._amount;

        if (newAmount >= 0) amount = newAmount;
        else return false;

        return true;
    }

    item_map _itemMap;
    transaction_map _userMap;
    const std::string_view _tId;
    mutable std::shared_mutex transactionItemMutex;
};

#endif