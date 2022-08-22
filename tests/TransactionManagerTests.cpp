#include <TransactionManager.h>
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

TEST_F(TransactionManagerTests, addUsersManyTransactions) {
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::ADD_USER>("1", "preman"));
    EXPECT_TRUE(t.updateTransaction<TransactionManager::UserActions::ADD_USER>("2", "preman"));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}