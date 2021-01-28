.PHONY: compile library copyToExamples

library:
	g++-8 --std=c++2a -pthread -c src/*.cpp
	ar crf target/libMQ.a MQ_lib.o
	ar crf target/libMQ.a MessageHandler.o
	rm MQ_lib.o
	rm MessageHandler.o

clean:
	rm example

copyToExamples: library
	cp ./target/libMQ.a ../ExamplePrograms/lib
	cp ./src/*.hpp ../ExamplePrograms/lib