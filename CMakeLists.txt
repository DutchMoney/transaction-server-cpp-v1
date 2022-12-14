cmake_minimum_required(VERSION 3.10)

if(${CMAKE_VERSION} VERSION_LESS 3.23)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.23)
endif()

project(TransactionServer VERSION 1.0
    DESCRIPTION "Transaction Server Library that will be plugged into a websocket server"
    LANGUAGES CXX)

add_library(Transaction src/Transaction.cpp src/Transaction.h src/Item.cpp src/Item.h src/TransactionManager.cpp src/TransactionManager.h src/Instruction.cpp src/Instruction.h)
set_property(TARGET Transaction
    PROPERTY CXX_STANDARD 17)
    
option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
