#include <TransactionManager.h>
#include <Transaction.h>
#include <gtest/gtest.h>

struct TransactionManagerTests : public testing::Test {
    TransactionManager t {};

    void SetUp() {
        t.addTransaction("1", {{{"apple", 5, 5}, {"banana", 10, 1}}});
        t.addTransaction("2", {{{"apple", 10, 5}, {"banana", 7, 9}}});
    }

    void TearDown() {

    }
};

// Test: Add users to multiple transactions
// Description: Adding users to multiple transactions and checking if added correctly
TEST_F(TransactionManagerTests, addUsersManyTransactions) {
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::ADD_USER>("1", "preman"));
    auto t1Map = t.printTransaction("1");
    EXPECT_TRUE(t1Map.find("preman") != t1Map.end());

    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::ADD_USER>("2", "preman2"));
    auto t2Map = t.printTransaction("2");
    EXPECT_TRUE(t2Map.find("preman2") != t2Map.end());
}

// Test: Check check users exist, many transactions
// Description: Adding users and validating the check function to see if works for valid and invalid users for many transactions
TEST_F(TransactionManagerTests, checkUsersManyTransactions) {
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::ADD_USER>("1", "preman"));
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("1", "preman"));
    EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("1", "preman2"));

    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::ADD_USER>("2", "preman"));
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("2", "preman"));
    EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("2", "preman2"));
}

// Test: Check remove users, many transactions
// Description: Add users, remove and check if gone, also remove invalid users
TEST_F(TransactionManagerTests, removeUsersManyTransactions) {

    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::ADD_USER>("1", "preman"));
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("1", "preman"));
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::REMOVE_USER>("1", "preman"));
    EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("1", "preman"));
    EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::REMOVE_USER>("1", "preman2"));
    EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("1", "preman2"));

    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::ADD_USER>("2", "preman"));
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("2", "preman"));
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::REMOVE_USER>("2", "preman"));
    EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("2", "preman"));
    EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::REMOVE_USER>("2", "preman2"));
    EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::CHECK_USER>("2", "preman2"));
}

// Test: Check add items valid amount
// Description: Add items to a user created and check if correct items remain in unused and users
TEST_F(TransactionManagerTests, addValidItemsUsersManyTransaction) {

    t.updateTransaction<TransactionManager::UserActions::ADD_USER>("1", "preman");
    bool res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>("1", "preman", {"apple", 2, 5});
    ASSERT_TRUE(res);

    auto tMap1 = t.printTransaction("1");
    EXPECT_EQ(std::get<0>(tMap1.find("preman")->second.find("apple")->second), 2);
    EXPECT_EQ(std::get<0>(tMap1.find("unused")->second.find("apple")->second), 3);
}

// Test: Check add items invalid amount
// Description: Add invalid items to a user and check unused to ensure items are conserved 
TEST_F(TransactionManagerTests, addInvalidItemsUsersManyTransactions) {

    t.updateTransaction<TransactionManager::UserActions::ADD_USER>("1", "preman");

    bool res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>("1", "preman", {"apple", 6, 5});
    ASSERT_FALSE(res);

    auto tMap1 = t.printTransaction("1");
    EXPECT_TRUE(tMap1.find("preman")->second.find("apple") == tMap1.find("preman")->second.end());
    EXPECT_EQ(std::get<0>(tMap1.find("unused")->second.find("apple")->second), 5);


    res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>("1", "preman", {"apple", -1, 5});
    ASSERT_FALSE(res);

    tMap1 = t.printTransaction("1");
    EXPECT_TRUE(tMap1.find("preman")->second.find("apple") == tMap1.find("preman")->second.end());
    EXPECT_EQ(std::get<0>(tMap1.find("unused")->second.find("apple")->second), 5);

//  TRANSACTION 2
    t.updateTransaction<TransactionManager::UserActions::ADD_USER>("2", "preman");

    res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>("2", "preman", {"apple", 11, 5});
    ASSERT_FALSE(res);

    tMap1 = t.printTransaction("2");
    EXPECT_TRUE(tMap1.find("preman")->second.find("apple") == tMap1.find("preman")->second.end());
    EXPECT_EQ(std::get<0>(tMap1.find("unused")->second.find("apple")->second), 10);


    res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>("2", "preman", {"apple", -1, 5});
    ASSERT_FALSE(res);

    tMap1 = t.printTransaction("2");
    EXPECT_TRUE(tMap1.find("preman")->second.find("apple") == tMap1.find("preman")->second.end());
    EXPECT_EQ(std::get<0>(tMap1.find("unused")->second.find("apple")->second), 10);
}

// Test:: Check add items to invalid user
TEST_F(TransactionManagerTests, addValidItemsInvalidUserManyTransactions) {
    
}

// TEST_F( TransactionManagerTests, updateItemPriceManyTransactions) {
    
//     t.updateTransaction<TransactionManager::UserActions::ADD_USER>("1", "preman");


// }

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}