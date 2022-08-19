#include "TransactionManager.h"

template <>
bool TransactionManager::updateTransaction<TransactionManager::UserActions::CHECK_USER>(const std::string& tId, const std::string& userId) {
    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end())  return false;

    return transIt->second.isUserInTransaction(userId);
}

template <>
bool TransactionManager::updateTransaction<TransactionManager::UserActions::ADD_USER>(const std::string& tId, const std::string& userId) {
    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end())  return false;

    return transIt->second.addUser(userId);
}

template<>
bool TransactionManager::updateTransaction<TransactionManager::UserActions::REMOVE_USER>(const std::string& tId, const std::string& userId) {

    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end())  return false;

    return transIt->second.removeUser(userId);
}

template <>
bool TransactionManager::updateTransaction<TransactionManager::UserActions::UPDATE_ITEM_PRICE>(const std::string& tId, const std::string& userId, const std::string& itemName, float itemPrice) {

    auto transIt = _transactionManagerMap.find(tId);
    if (transIt == _transactionManagerMap.end())  return false;

    return transIt->second.updateItemPrice(itemName, itemPrice);
}