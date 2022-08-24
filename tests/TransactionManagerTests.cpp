#include <TransactionManager.h>
#include <Transaction.h>
#include <gtest/gtest.h>

struct TransactionManagerTests : public testing::Test {
    TransactionManager t {};
    std::vector<std::string_view> tIds{{"1", "2"}};
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
//  TRANSACTION 1
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

// Test: Check add items to invalid user
// Description: Add valid items to invalid user, ensure unused items conserved
TEST_F(TransactionManagerTests, addValidItemsInvalidUserManyTransactions) {
    auto transactionWork = [&](const std::string_view tId) {
        t.updateTransaction<TransactionManager::UserActions::ADD_USER>(tId, "preman");
        int origAmountApples = std::get<0>(t.printTransaction(tId).find("unused")->second.find("apple")->second);

        bool res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>(tId, "preman2", {"apple",  3, 5});
        ASSERT_FALSE(res);

        EXPECT_EQ(std::get<0>(t.printTransaction(tId).find("unused")->second.find("apple")->second), origAmountApples);
    };

    for (auto& tId : tIds) transactionWork(tId);

}

// Test: Check remove valid item from user
// Description: Add a user with some items, remove and ensure unused has original amount and user has no more
TEST_F( TransactionManagerTests, removeValidItemsUsersManyTransactions ) {
    auto transactionWork = [&](const std::string_view tId) {

        t.updateTransaction<TransactionManager::UserActions::ADD_USER>(tId, "preman");
        
        int origAmountApples = std::get<0>(t.printTransaction(tId).find("unused")->second.find("apple")->second);
        
        bool res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>(tId, "preman", {"apple", 2, 5});
        ASSERT_TRUE(res);

        res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::REMOVE>(tId, "preman", {"apple", 2, 5});
        ASSERT_TRUE(res);

        auto tMap = t.printTransaction(tId);
        EXPECT_EQ(std::get<0>(tMap.find("unused")->second.find("apple")->second), origAmountApples);
        EXPECT_TRUE(tMap.find("preman")->second.find("apple") == tMap.find("preman")->second.end());
    };

    for (auto& tId : tIds) transactionWork(tId);
}

// Test: Check remove items invalid amount
// Description: Check removing invalid number of items from a user and conserving item state
TEST_F( TransactionManagerTests, removeInvalidItemsUsersManyTransactions) {
    auto transactionWork = [&](const std::string_view tId) {

        t.updateTransaction<TransactionManager::UserActions::ADD_USER>(tId, "preman");
        
        bool res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>(tId, "preman", {"apple", 2, 5});
        ASSERT_TRUE(res);

        int origAmountApples = std::get<0>(t.printTransaction(tId).find("unused")->second.find("apple")->second);

        res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::REMOVE>(tId, "preman", {"apple", -1, 5});
        ASSERT_FALSE(res);

        res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::REMOVE>(tId, "preman", {"apple", 25, 5});
        ASSERT_FALSE(res);

        EXPECT_EQ(origAmountApples, std::get<0>(t.printTransaction(tId).find("unused")->second.find("apple")->second));

    };
    
    for (auto& tId : tIds) transactionWork(tId);
}

// Test: Check update item price
// Description: Update price of an item and check if all users and unused have updated price, for many transactions
TEST_F( TransactionManagerTests, updateItemPriceManyTransactions) {
    auto transactionWork = [&](const std::string_view tId, float price) {

        t.updateTransaction<TransactionManager::UserActions::ADD_USER>(tId, "preman");
        
        bool res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>(tId, "preman", {"apple",  3, 5});
        ASSERT_TRUE(res);

        EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::UPDATE_ITEM_PRICE>(tId, "preman", "apple", price));

        auto tMap = t.printTransaction(tId);

        for (auto& [key, user] : tMap) {
            auto userIt = user.find("apple");
            if (userIt == user.end()) continue;

            auto& [_amount, _price] = userIt->second;

            EXPECT_EQ(_price, price);
        }

    };

    for (auto& tId : tIds) transactionWork(tId, 8);
}

TEST_F( TransactionManagerTests, updateItemInvalidPriceManyTransactions) {
    auto transactionWork = [&](const std::string_view& tId, float price) {

        t.updateTransaction<TransactionManager::UserActions::ADD_USER>(tId, "preman");

        float origPriceApples = std::get<1>(t.printTransaction(tId).find("unused")->second.find("apple")->second);
        
        bool res = t.updateTransaction<TransactionManager::UserActions::UPDATE_USER_ITEMS, Transaction::UpdateType::ADD>(tId, "preman", {"apple",  3, 5});
        ASSERT_TRUE(res);

        EXPECT_FALSE(t.updateTransaction<TransactionManager::UserActions::UPDATE_ITEM_PRICE>(tId, "preman", "apple", price));

        auto tMap = t.printTransaction(tId);

        for (auto& [key, user] : tMap) {
            auto userIt = user.find("apple");
            if (userIt == user.end()) continue;

            auto& [_amount, _price] = userIt->second;

            EXPECT_EQ(_price, origPriceApples);
        }
    };

    for (auto& tId : tIds) transactionWork(tId, -1);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}