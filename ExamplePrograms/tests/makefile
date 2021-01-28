.PHONY: compile

all: messageDeletion dataOverload dataConsistency queuesList lowBandwith userList

messageDeletion:
	g++-8 --std=c++2a -O0 -pthread -c MessageDeletionTest/main.cpp
	g++-8 --std=c++2a -O0 -pthread main.o -L../lib -lMQ -o MessageDeletionTest/main
	rm main.o

dataOverload:
	g++-8 --std=c++2a -O0 -pthread -c dataOverloadTest/main.cpp
	g++-8 --std=c++2a -O0 -pthread main.o -L../lib -lMQ -o dataOverloadTest/main
	rm main.o

dataConsistency:
	g++-8 --std=c++2a -O0 -pthread -c dataConsistencyTest/main.cpp
	g++-8 --std=c++2a -O0 -pthread main.o -L../lib -lMQ -o dataConsistencyTest/main
	rm main.o

queuesList:
	g++-8 --std=c++2a -O0 -pthread -c queuesListTest/creator.cpp
	g++-8 --std=c++2a -O0 -pthread creator.o -L../lib -lMQ -o queuesListTest/creator
	rm creator.o

	g++-8 --std=c++2a -O0 -pthread -c queuesListTest/reader.cpp
	g++-8 --std=c++2a -O0 -pthread reader.o -L../lib -lMQ -o queuesListTest/reader
	rm reader.o

lowBandwith:
	g++-8 --std=c++2a -O0 -pthread -c lowBandwithTest/creator.cpp
	g++-8 --std=c++2a -O0 -pthread creator.o -L../lib -lMQ -o lowBandwithTest/creator
	rm creator.o

	g++-8 --std=c++2a -O0 -pthread -c lowBandwithTest/reader.cpp
	g++-8 --std=c++2a -O0 -pthread reader.o -L../lib -lMQ -o lowBandwithTest/reader
	rm reader.o

userList:
	g++-8 --std=c++2a -O0 -pthread -c userListTest/creator.cpp
	g++-8 --std=c++2a -O0 -pthread creator.o -L../lib -lMQ -o userListTest/creator
	rm creator.o

	g++-8 --std=c++2a -O0 -pthread -c userListTest/reader.cpp
	g++-8 --std=c++2a -O0 -pthread reader.o -L../lib -lMQ -o userListTest/reader
	rm reader.o

clean:
	rm example