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

    ConnectionHandler* connectionHandler = new ConnectionHandler(host, port);
    if (!connectionHandler->connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    bool shouldTerminate = false;
    clientTask task1(connectionHandler, shouldTerminate);// check wtf?
    serverTask task2(connectionHandler, shouldTerminate);
    std::thread th1(&clientTask::run, &task1);
    std::thread th2(&serverTask::run, &task2);
    th1.detach();
    th2.detach();
    delete connectionHandler; //possible memory leak
    return 0;
}