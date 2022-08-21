#include "TransactionManager.h"

template <>
bool TransactionManager::updateTransaction<TransactionManager::UserActions::CHECK_USER>(const std::string_view& tId, const std::string_view& userId) {
    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end())  return false;

    return transIt->second.isUserInTransaction(userId);
}

template <>
bool TransactionManager::updateTransaction<TransactionManager::UserActions::ADD_USER>(const std::string_view& tId, const std::string_view& userId) {
    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end())  return false;

    return transIt->second.addUser(userId);
}

template<>
bool TransactionManager::updateTransaction<TransactionManager::UserActions::REMOVE_USER>(const std::string_view& tId, const std::string_view& userId) {

    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end())  return false;

    return transIt->second.removeUser(userId);
}

template <>
bool TransactionManager::updateTransaction<TransactionManager::UserActions::UPDATE_ITEM_PRICE>(const std::string_view& tId, const std::string_view& userId, const std::string_view& itemName, float itemPrice) {

    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end())  return false;

    return transIt->second.updateItemPrice(itemName, itemPrice);
}

bool TransactionManager::addTransaction(std::string_view tId, Transaction&& t) {
     
    auto transIt = _transactionManagerMap.find(tId);
    if (transIt != _transactionManagerMap.end()) return false;

    auto [it, isInserted] = _transactionManagerMap.insert({tId, std::move(t)});

    return isInserted;
}

bool TransactionManager::removeTransaction(const std::string_view& tId) {
    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end()) return false;

    bool isRemoved = _transactionManagerMap.erase(tId);

    return isRemoved; 
}

transaction_map TransactionManager::printTransaction(const std::string_view& tId) {
    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end()) return {};

    return transIt->second.getTransactionMap();
}