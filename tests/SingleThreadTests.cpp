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
    
    t->addUserIfNotExists("preman");
    t->addUserIfNotExists("preman2");
    t->addUserIfNotExists("preman3");

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

    t->removeUser("preman");

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

    EXPECT_EQ(t->isUserInTransaction("preman"), 1);
    EXPECT_EQ(t->isUserInTransaction("preman2"), 1);
    EXPECT_EQ(t->isUserInTransaction("preman3"), 1);

    t->removeUser("preman");

    EXPECT_EQ(t->isUserInTransaction("preman"), 0);
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

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

    return 0;
}