
#include <Transaction.h>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include <thread>
#include <iostream>

struct MultiThreadTests : public testing::Test {
    
    Transaction t {"1", {{"apple", 5, 5}, {"banana", 10, 1}}};

    void SetUp() {

        t.addUser("preman");
        t.addUser("preman2");
        t.addUser("preman3");
        t.addUser("preman4");
        t.addUser("preman5");
        t.addUser("preman6");
        t.addUser("preman7");
        t.addUser("preman8");
        t.addUser("preman9");
        t.addUser("preman10");
    }

    void TearDown() {
    }
};

TEST_F(MultiThreadTests, addUserInvalidMultiItems) {
    auto addItemsToUser = [this] (const std::string& userId, bool expected) {
        std::cout << "Acquire " << userId << std::endl;

        bool res = t.updateUserItem<Transaction::UpdateType::ADD>(userId, {"apple", 2, 5});
        EXPECT_EQ(res, expected);
        std::cout << "Release " << userId << std::endl;
    };

    auto checkMapByKey = [this](std::string userId, std::string item_key, int amount) {
        std::cout << "Reading " << userId << std::endl;
        EXPECT_EQ(std::get<0>(t.getTransactionMap().find(userId)->second.find(item_key)->second), amount);
        std::cout << "Read " << userId << std::endl;
    };

    auto t1 = std::thread(addItemsToUser, "preman", true);
    auto t3 = std::thread(checkMapByKey, "preman", "apple", 2);
    auto t4 = std::thread(checkMapByKey, "unused", "apple", 3);
    auto t2 = std::thread(addItemsToUser, "preman2", true);
    // auto t3 = std::thread(addItemsToUser, "preman3", false);
    // auto t4 = std::thread(addItemsToUser, "preman4", false);
    // auto t5 = std::thread(addItemsToUser, "preman5", false);
    // auto t6 = std::thread(addItemsToUser, "preman6", false);
    // auto t7 = std::thread(addItemsToUser, "preman7", false);
    // auto t8 = std::thread(addItemsToUser, "preman8", false);
    // auto t9 = std::thread(addItemsToUser, "preman9", false);
    // auto t10 = std::thread(addItemsToUser, "preman10", false);


    t1.join();
    t3.join();
    t4.join();
    t2.join();
    // t9.join();
    // t10.join();
    // t8.join();
    // t7.join();
    // t6.join();
    // t5.join();
    // t4.join();
    // t3.join();


    auto tMap = t.getTransactionMap();

    EXPECT_EQ(std::get<0>(tMap.find("unused")->second.find("apple")->second), 1);
    EXPECT_EQ(std::get<0>(tMap.find("preman")->second.find("apple")->second), 2);
    EXPECT_EQ(std::get<0>(tMap.find("preman2")->second.find("apple")->second), 2);
    // EXPECT_EQ(tMap.find("preman2")->second.find("apple"), tMap.find("preman2")->second.end());
    EXPECT_EQ(tMap.find("preman3")->second.find("apple"), tMap.find("preman3")->second.end());
    EXPECT_EQ(tMap.find("preman4")->second.find("apple"), tMap.find("preman4")->second.end());
    EXPECT_EQ(tMap.find("preman5")->second.find("apple"), tMap.find("preman5")->second.end());
    EXPECT_EQ(tMap.find("preman6")->second.find("apple"), tMap.find("preman6")->second.end());
    EXPECT_EQ(tMap.find("preman7")->second.find("apple"), tMap.find("preman7")->second.end());
    EXPECT_EQ(tMap.find("preman8")->second.find("apple"), tMap.find("preman8")->second.end());
    EXPECT_EQ(tMap.find("preman9")->second.find("apple"), tMap.find("preman9")->second.end());
    EXPECT_EQ(tMap.find("preman10")->second.find("apple"), tMap.find("preman10")->second.end());

}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}