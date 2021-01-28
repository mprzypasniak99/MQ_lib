.PHONY: compile

all: messageDeletion dataOverload

messageDeletion:
	g++-8 --std=c++2a -O0 -pthread -c MessageDeletionTest/main.cpp
	g++-8 --std=c++2a -O0 -pthread main.o -L../lib -lMQ -o MessageDeletionTest/main
	rm main.o

dataOverload:
	g++-8 --std=c++2a -O0 -pthread -c dataOverloadTest/main.cpp
	g++-8 --std=c++2a -O0 -pthread main.o -L../lib -lMQ -o dataOverloadTest/main
	rm main.o

clean:
	rm example