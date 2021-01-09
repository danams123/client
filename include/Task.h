#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

#include "../include/connectionHandler.h"

class Task{
public:
    //implementing the rule of 3 because of the connectionHandler pointer
    Task(ConnectionHandler *connectionHandler);
    virtual ~Task();
    Task(const Task& other);
    const Task& operator = (const Task& other);

    void run();

private:
    //in order to pass the connectionHandler by ref
    ConnectionHandler* connectionHandler;
};

#endif //CLIENT_TASK_H
