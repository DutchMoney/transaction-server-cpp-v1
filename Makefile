
transaction_test: item.o transaction.o main.o
	g++ -std=c++17 -o transaction_test item.o transaction.o main.o

main.o: src/main.cpp
	g++ -std=c++17 -c src/main.cpp

item.o: src/Item.h src/Item.cpp
	g++ -std=c++17 -c src/Item.cpp

transaction.o: src/Transaction.h src/Transaction.cpp
	g++ -std=c++17 -c src/Transaction.cpp

