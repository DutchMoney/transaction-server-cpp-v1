#include <Transaction.h>
#include <vector>
#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include <iostream>

struct TransactionTest : public testing::Test {
    Transaction* t;
    void SetUp() {
        t = new Transaction{{{"apple", 5, 5}, {"banana", 10, 1}}};
    }

    void TearDown() {
        delete t;
    }
};

// Test: Add users
// Description: Adding users and checking if added correctly
TEST_F(TransactionTest, addUser) {
    
    EXPECT_EQ(t->addUserIfNotExists("preman"), 1);
    EXPECT_EQ(t->addUserIfNotExists("preman2"), 1);
    EXPECT_EQ(t->addUserIfNotExists("preman3"), 1);

    EXPECT_FALSE(t->addUserIfNotExists("preman"));

    std::vector<std::string> users;
    for (auto& [user, items] : t->getTransactionMap()) {
        if (user == "unused") continue;

        users.push_back(user);
    }

    std::vector<std::string> expect {"preman3", "preman2", "preman"};

    EXPECT_EQ(users, expect);
}

//Test: Remove Users
//Description: Removing users and checking if removed correctly
TEST_F(TransactionTest, removeUser) {

    t->addUserIfNotExists("preman");
    t->addUserIfNotExists("preman2");
    t->addUserIfNotExists("preman3");

    EXPECT_TRUE(t->removeUser("preman"));
    EXPECT_FALSE(t->removeUser("james"));

    std::vector<std::string> users;
    for (auto& [user, items] : t->getTransactionMap()) {
        if (user == "unused") continue;

        users.push_back(user);
    }

    std::vector<std::string> expect {"preman3", "preman2"};

    EXPECT_EQ(users, expect);
}

//Test: Check isUserInTransaction
//Description: Assuming add users and remove users correct, check if can identify valid and invalid users
TEST_F(TransactionTest, isUserInTransaction) {

    t->addUserIfNotExists("preman");
    t->addUserIfNotExists("preman2");
    t->addUserIfNotExists("preman3");

    EXPECT_TRUE(t->isUserInTransaction("preman"));
    EXPECT_TRUE(t->isUserInTransaction("preman2"));
    EXPECT_TRUE(t->isUserInTransaction("preman3"));

    EXPECT_FALSE(t->isUserInTransaction("james"));

    t->removeUser("preman");

    EXPECT_FALSE(t->isUserInTransaction("preman"));
}

//Test: Check valid unused price update
//Description: Check if new valid price given to a valid item updates across all users
TEST_F(TransactionTest, updateItemPriceValid) {
    EXPECT_EQ(std::get<1>(t->getTransactionMap().find("unused")->second.find("apple")->second), 5);

    bool status = t->updateItemPrice("apple", 9.5);

    EXPECT_TRUE(status);

    auto& userMap = t->getTransactionMap();

    for (auto& [user, itemMap] : t->getTransactionMap()) {
        EXPECT_EQ(std::get<1>(itemMap.find("apple")->second), 9.5);
    }
}

//Test: Check invalid unused price update
//Description: Check if new invalid price given to a valid item maintains state and correct status
TEST_F(TransactionTest, updateItemPriceInvalid) {

    EXPECT_EQ(std::get<1>(t->getTransactionMap().find("unused")->second.find("apple")->second), 5);

    bool status = t->updateItemPrice("apple", -9.5);

    EXPECT_FALSE(status);

    for (auto& [user, itemMap] : t->getTransactionMap()) {
        EXPECT_EQ(std::get<1>(itemMap.find("apple")->second), 5);
    }
}

//Test: check invalid unused, valid price update
//Description: Check if new valid price given to invalid item gives correct status
TEST_F(TransactionTest, updateInvalidItemPrice) {

    bool status = t->updateItemPrice("pear", 9.5);

    EXPECT_FALSE(status);
}

//Test: check invalid user, valid add items
// Description: Check if adding items to an invalid user doesn't create new user or changed the unused item amounts and gives correct status
TEST_F(TransactionTest, addInvalidUserItems) {


    t->addUserIfNotExists("preman");
    t->addUserIfNotExists("preman2");
    t->addUserIfNotExists("preman3");

    EXPECT_FALSE(t->updateUserItem<Transaction::UpdateType::ADD>("preman4", {"apple", 3, 5}));

    EXPECT_TRUE(std::get<0>(t->getTransactionMap().find("unused")->second.find("apple")->second) == 5);

    EXPECT_FALSE(t->isUserInTransaction("preman4"));
}

//Test: check invalid user, valid remove items
//Description: Check if removing items from invalid user doens't create new user or change the unused item amounts and gives correct status
TEST_F(TransactionTest, removeInvalidUserItems) {

    t->addUserIfNotExists("preman");
    t->addUserIfNotExists("preman2");
    t->addUserIfNotExists("preman3");

    EXPECT_FALSE(t->updateUserItem<Transaction::UpdateType::REMOVE>("preman4", {"apple", 0, 5}));

    EXPECT_TRUE(std::get<0>(t->getTransactionMap().find("unused")->second.find("apple")->second) == 5);

    EXPECT_FALSE(t->isUserInTransaction("preman4"));
}

//Test: check valid user, add valid items
//Description: Check if some of an item can be added to an empty user, all statuses correct and corret amounts for user and unused 
TEST_F(TransactionTest, addEmptyUserValidItems) {

    t->addUserIfNotExists("preman");
    t->addUserIfNotExists("preman2");
    t->addUserIfNotExists("preman3");

    EXPECT_TRUE(t->updateUserItem<Transaction::UpdateType::ADD>("preman", {"apple", 3, 5}));

    EXPECT_EQ(std::get<0>(t->getTransactionMap().find("preman")->second.find("apple")->second), 3);
    EXPECT_EQ(std::get<0>(t->getTransactionMap().find("unused")->second.find("apple")->second), 2);

}

//Test: check valid user with items already, add more items
//Description: Check if an item can be added to a user that already contains that item, all statuses correct and correct amounts for user and unused
TEST_F(TransactionTest, addExistingUserValidItems) {

    t->addUserIfNotExists("preman");
    t->addUserIfNotExists("preman2");
    t->addUserIfNotExists("preman3");

    t->updateUserItem<Transaction::UpdateType::ADD>("preman", {"apple", 3, 5});

    EXPECT_TRUE(t->updateUserItem<Transaction::UpdateType::ADD>("preman", {"apple", 1, 5}));


    EXPECT_EQ(std::get<0>(t->getTransactionMap().find("preman")->second.find("apple")->second), 4);
    EXPECT_EQ(std::get<0>(t->getTransactionMap().find("unused")->second.find("apple")->second), 1);
}

//Test: check valid user with items already, add rest of items
//Description: check if th remaining in unused of an item can be added to a user, all statuses correct and unusued should be 0, correct amounts for user also
TEST_F(TransactionTest, addExistingUserValidAllItems) {

    t->addUserIfNotExists("preman");
    t->addUserIfNotExists("preman2");
    t->addUserIfNotExists("preman3");

    t->updateUserItem<Transaction::UpdateType::ADD>("preman", {"apple", 3, 5});

    EXPECT_TRUE(t->updateUserItem<Transaction::UpdateType::ADD>("preman", {"apple", 2, 5}));

    auto tMap = t->getTransactionMap();
    auto applesIt = tMap.find("unused")->second.find("apple");

    ASSERT_TRUE(applesIt != tMap.find("unused")->second.end());
    
    EXPECT_EQ(std::get<0>(t->getTransactionMap().find("preman")->second.find("apple")->second), 5);
    EXPECT_EQ(std::get<0>(t->getTransactionMap().find("unused")->second.find("apple")->second), 0);
}



int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

    return 0;
}