# transaction-server-cpp-v1
An OOP version with CPP of the transaction srever, for benchmarking and self-learning purposes

## Build Instructions
1. Update all git submodules first using ```git submodule update --recursive --remote```
2. Run ```cmake -S . -B build``` in the root directory to create build folder and generate Makefile.
3. Run ```cmake --build build``` in the root directory to generate the ```app``` binary.
4. Run ```./build/app``` 

## Progress
### Transaction - Single Threaded
1. Create Transaction with list of items
2. Check if user exists in transaction
3. Add/remove user to transaction
4. Update user's item map
5. Check if item is unused (validate amount and price)
6. Check if user has item (validate amount and price)
7. Update unused item map
8. Get copy of entire transaction as a ```transaction_map```, unused items in ```unused``` key
9. Print out transaction for easy visual debugging

### Build Tools / Dependencies / Executables
1. Added CMake for easier compilation

### Tests 
#### Transaction - Single Threaded
1. ```addUser``` - Add users
2. ```removeUser``` - Add then remove users + check if items are returned
3. ```isUserInTransaction``` - add users, check valid user, check invalid user.
4. ```updateItemPrice``` - check updating non-existent item, valid price, invalid price, check all user's price
5. ```updateUserItem``` - add users, check adding/remove non existent items. check adding/remove valid amount of items. check adding/removing invalid number of items

#### TransactionManager - Single Threaded
1. ```updateTransaction (ADD_USER)``` - Add users to a specific transaction by tId, needs to be existing tId and non existing userId
2. ```updateTransaction (CHECK_USER)``` - Check if a user exists in a transaction or not, should return correct bool
3. ```updateTransaction (REMOVE_USER)``` - Remove users from a specific transaction by tId, needs to  be existing tId, and existing userId
4. ```updateTransaction (UPDATE_USER_ITEMS)``` - Add/remove valid/invalid items from valid/invald users, check transaction map conserved in all cases
5. ```updateTranasaction (UPDATE_ITEM_PRICE)``` - Update an item's price and unused + all users for that tId should correlate

## Current Development
- [X] Add Google Test to CMake
- [X] Add single threaded tests
- [X] Add all tests and their description to README
- [X] Test out adding multiple items to multiple users concurrently
- [X] Add read/write locks per item/user in transaction (only added a reader writer lock for the item map)
- [X] Add a Transaction Manager to hold a group of transactions and perform actions on them
- [X] Add test for all TransactionManager methods to ensure edge cases are covered
## Future
### Threading
1. Test out adding multiple items to multiple users concurrently
2. Add read/write locks per item/user in transaction - Currently exists locks for ```item_map```

### General Design
1. Replace tuples with items/user objects
2. Replace all current ```strings``` with ```string_views``` - DONE
3. Transaction manager to add a wrapper to update multiple transactions - DONE but requires constructor to initialize, and a structure messages
4. Add Factory class for creating transactions (possible empty transaction, user id, transaction id, think about merging in the future but less important)
5. User permissions, user can only update their own items
6. Item UUIDs
7. Transaction UUIDs

### Websockets
1. Deal with making/parsing small JSON objects & making message formats for actions
2. Add Boost Beast and Booost ASIO as dependencies to CMake for future websockets
3. Think of big pictue product flow of a transacton (may require transaction states like ```finishTransaction``` etc.)
4. Create high level framework on top of Boost Beast to run a Websocket Server, create server message formats
4. JWT / Authentication / Authorization for Users

## Thoughts
- Need to use acquire-release semantic to decrease lock usage as much as possible - starting with a shared mutex to allow concurrent reads and writes, but eventually need to move to TBB ```concurrent_hash_map```
- Prefer task bansed over threasd based concurrency
- I'm basically building a cache, soon need to start thinking about the architecture and how to fit it in, otherwise I'm basically rebuilding redis