
transaction_test: link
	g++ -std=c++17 -fpic -o transaction_test main.o Transaction.o Item.o

link:
	g++ -std=c++17 -c src/main.cpp src/Transaction.cpp src/Item.cpp
# main.o: src/main.cpp
# 	g++ -std=c++17 -c src/main.cpp

# Item.o: src/Item.h src/Item.cpp
# 	g++ -std=c++17 -c src/Item.cpp

# Transaction.o: src/Transaction.h src/Transaction.cpp
# 	g++ -std=c++17 -c src/Transaction.cpp

