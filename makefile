CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude -lboost_system -lboost_thread -pthread
LDFLAGS:=-lboost_system -lboost_thread -pthread

all: bin/BGRSclient

bin/BGRSclient: bin/BGRSclient.o bin/connectionHandler.o bin/Task.o

	@echo 'Building target: BGRSclient'
	@echo 'Invoking: C++ Linker'
	g++ -Wall -Weffc++ -o bin/BGRSclient bin/BGRSclient.o bin/connectionHandler.o bin/Task.o $(LDFLAGS)
	@echo 'Finished building target: BGRSclient'
	@echo ' '

bin/BGRSclient.o: src/BGRSclient.cpp
	g++ $(CFLAGS) -o bin/BGRSclient.o src/BGRSclient.cpp

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Task.o: src/Task.cpp
	g++ $(CFLAGS) -o bin/Task.o src/Task.cpp


.PHONY: clean
clean:
	rm -f bin/*