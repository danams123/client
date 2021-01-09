#include <stdlib.h>
#include "../include/connectionHandler.h"
#include "../include/Task.h"
#include <iostream>
#include <thread>


int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);

    //so it would be passed by ref to the Task
    ConnectionHandler *connectionHandler = new ConnectionHandler(host, port);

    if (!connectionHandler->connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        connectionHandler->close();
        delete connectionHandler;
        return 1;
    }
    //initializing the thread of Task
    Task task1(connectionHandler);
    std::thread th1(&Task::run, &task1);
    //keep going until finish is true
    while (!connectionHandler->getFinish()) {
        std::string answer;
        // getting the line from the server
        if (!connectionHandler->getLine(answer)) {
            connectionHandler->close();
            delete connectionHandler;
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        int len = answer.length();
        answer.resize(len - 1);
        // output answer
        std::cout << answer << std::endl;
        //if the answe is ACK 4 than its time to finish the program
        if (answer == "ACK 4") {
            th1.join();
            connectionHandler->setFinish();
            connectionHandler->close();
            std::cout << "Exiting...\n" << std::endl;
        }
    }
    delete connectionHandler;
    return 0;
}