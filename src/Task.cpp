#include <cstring>
#include <vector>
#include "../include/Task.h"

Task::Task(ConnectionHandler *connectionHandler): connectionHandler(connectionHandler){}

Task::~Task(){
    if(connectionHandler != nullptr) {
        connectionHandler = nullptr;
    }
}
//TODO put in the right way so we will use the same object
Task::Task(const Task &other): connectionHandler(){
    connectionHandler = other.connectionHandler;

}

const Task& Task::operator=(const Task &other) {
    if(this != &other){
        if(connectionHandler != nullptr) {
            delete connectionHandler;
        }
        connectionHandler = other.connectionHandler;
    }
    return *this;
}


void Task::run() {
    // keep going until shouldTerminate is true
    while (!connectionHandler->getTerminate()){
        // getting the line from the keyboard
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        //encoding the chars given from the user
        std::vector<char> vec = connectionHandler->encode(buf);
        //if the user didnt put a valid word
        if(vec.size() == 0){
        continue;
         }
        //sending the vector as a char array
        char* output = new char[vec.size()];
        for (unsigned int i = 0; i < vec.size(); ++i) {
            output[i] = vec[i];
        }
        int len = vec.size();
        vec.clear();
        //sending the bytes to the server
        if (!connectionHandler->sendBytes(output, len)) {
            connectionHandler->close();
            delete connectionHandler;
            delete[] output;
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        delete[] output;
    }
}
