#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H
#include <string>
#include <unordered_map>
#include "Transaction.h"
#include "Item.h"

class TransactionManager {
    
public:
    enum UserActions {
        CHECK_USER,
        ADD_USER,
        REMOVE_USER,
        UPDATE_ITEM_PRICE,
        UPDATE_USER_ITEMS,
        UPDATE_UNUSED_ITEMS
    };

    template <UserActions T> 
    bool updateTransaction(const std::string& tId, const std::string& userId);

    template <UserActions T> 
    bool updateTransaction(const std::string& tId, const std::string& userId, const std::string& itemName, float itemPrice);

    template <UserActions T, Transaction::UpdateType U> 
    bool updateTransaction(const std::string& tId, const std::string& userId, Item&& item) {
        if (T != UPDATE_USER_ITEMS) return false;

        auto transIt = _transactionManagerMap.find(tId);
        if (transIt == _transactionManagerMap.end())  return false;

        return transIt->second.updateUserItem<U>(userId, item);
    }

    bool addTransaction(const std::string& tId, Transaction&& t);

    bool removeTransaction(const std::string& tId);

    transaction_map printTransaction(const std::string& tId);

private:
    std::unordered_map<std::string_view, Transaction> _transactionManagerMap;

};

#endif