#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "TransactionManager.h"
#include "Item.h"

#define NULL_ITEM_NAME "IGNORE_ITEM"
class Instruction {
public:
    const TransactionManager::UserActions _action;
    const std::string_view _userId;
    const std::string_view _tId;
    const Item _item;

    template <TransactionManager::UserActions T>
    static Instruction instructionFactoryUser(std::string_view&& userId, std::string_view&& tId);

    template <TransactionManager::UserActions T>
    static Instruction instructionFactoryItem(std::string_view&& userId, std::string_view&& tId, Item&& item);

private:
    Instruction(TransactionManager::UserActions action ,std::string_view&& userId, std::string_view&& tId, Item&& item) : 
        _action(action), _userId(userId), _tId(tId), _item(item) {};
};
#endif