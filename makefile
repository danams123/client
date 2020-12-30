CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: Client

Client: bin/BGRSclient.o bin/connectionHandler.o bin/Task.o
	@echo 'Building target: Client'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/BGRSclient.o bin/connectionHandler.o bin/Task.o $(LDFLAGS)
	@echo 'Finished building target: Client'
	@echo ' '

bin/BGRSclient.o: src/BGRSclient.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/BGRSclient.o src/BGRSclient.cpp
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Task.o: src/Task.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Task.o: src/Task.cpp



.PHONY: clean
clean:
	rm -f bin/*
