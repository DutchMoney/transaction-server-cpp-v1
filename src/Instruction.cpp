#include "Instruction.h"

template<>
Instruction Instruction::instructionFactoryUser<TransactionManager::UserActions::CHECK_USER>(std::string_view&& userId, std::string_view&& tId) {
    return {TransactionManager::UserActions::CHECK_USER, std::move(userId), std::move(tId), Item{NULL_ITEM_NAME, 0, 0, Item::ItemState::IGNORED}};
}

template<>
Instruction Instruction::instructionFactoryUser<TransactionManager::UserActions::ADD_USER>(std::string_view&& userId, std::string_view&& tId) {
    return {TransactionManager::UserActions::ADD_USER, std::move(userId), std::move(tId), Item{NULL_ITEM_NAME, 0, 0, Item::ItemState::IGNORED}};
}

template<>
Instruction Instruction::instructionFactoryUser<TransactionManager::UserActions::REMOVE_USER>(std::string_view&& userId, std::string_view&& tId) {
    return {TransactionManager::UserActions::REMOVE_USER, std::move(userId), std::move(tId), Item{NULL_ITEM_NAME, 0, 0, Item::ItemState::IGNORED}};
}

template<>
Instruction Instruction::instructionFactoryItem<TransactionManager::UserActions::UPDATE_ITEM_PRICE>(std::string_view&& userId, std::string_view&& tId, Item&& item) {
    return {TransactionManager::UserActions::UPDATE_ITEM_PRICE, std::move(userId), std::move(tId), std::move(item)};
}

template<>
Instruction Instruction::instructionFactoryItem<TransactionManager::UserActions::UPDATE_UNUSED_ITEMS>(std::string_view&& userId, std::string_view&& tId, Item&& item) {
    return {TransactionManager::UserActions::UPDATE_UNUSED_ITEMS, std::move(userId), std::move(tId), std::move(item)};
}

template<>
Instruction Instruction::instructionFactoryItem<TransactionManager::UserActions::UPDATE_USER_ITEMS>(std::string_view&& userId, std::string_view&& tId, Item&& item) {
    return {TransactionManager::UserActions::UPDATE_USER_ITEMS, std::move(userId), std::move(tId), std::move(item)};
}