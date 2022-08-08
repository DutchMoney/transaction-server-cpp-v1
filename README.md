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
4. Update usr's item map
5. Check if item is unused (validate amount and price)
6. Check if user has item (validate amount and price)
7. Update unused item map
8. Get copy of entire transaction as a ```transaction_map```, unused items in ```unused``` key
9. Print out transaction for easy visual debugging

### Build Tools / Dependencies / Executables
1. Added CMake for easier compilation

## Current Development
- [ ] Add Google Test to CMake
- [ ] Add single threaded tests

## Future
### Testing
1. Add Google Test to CMake
2. Add a testing module
3. Add all tests and their description to README

### Threading
1. Add read/write locks per item/user in transaction
2. Test out adding multiple items to multiple users concurrently

### General Design
1. Replace all current ```strings``` with ```string_views```
2. Add Factory class for creating transactions (possible empty transaction, think about merging in the future but less important)
3. User permissions, user can only update their own items
4. Item UUIDs
5. Transaction UUIDs
6. Transaction manager to handle many transactions concurrently 

### Websockets
1. Add Boost Beast and Booost ASIO as dependencies to CMake for future websockets
2. Deal with making/parsing small JSON objects
3. Think of big pictue product flow of a transacton (may require transaction states like ```finishTransaction``` etc.)
4. Create high level framework on top of Boost Beast to run a Websocket Server, create server message formats
4. JWT / Authentication / Authorization for Users