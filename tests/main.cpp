#include <Transaction.h>
#include <vector>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestSuite, TestName) {
    ASSERT_TRUE(1 == 1);
}

int main(int argc, char** argv) {
    Transaction t {std::vector<Item> {{"apple", 5, 5}, {"banana", 10, 1}}}; 

    std::cout << t << std::endl;

    t.addUserIfNotExists("preman");
    bool sts  = t.updateUserItem<Transaction::UpdateType::ADD>("preman", {"apple", 2, 5});
    sts  = t.updateUserItem<Transaction::UpdateType::ADD>("preman", {"banana", 12, 1});
    sts  = t.updateUserItem<Transaction::UpdateType::ADD>("preman", {"banana", 10, 1});
    std::cout << t << std::endl;
    sts  = t.updateUserItem<Transaction::UpdateType::REMOVE>("preman", {"banana", 5, 1});

    std::cout << t << std::endl;

    sts = t.updateItemPrice("banana", 6);

    std::cout << t << std::endl;

    sts = t.removeUser("preman");
    std::cout << t << std::endl;

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

    return 0;
}