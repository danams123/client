#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

#include "../include/connectionHandler.h"

class Task{
public:
    Task(ConnectionHandler connectionhandler, bool shouldTerminate);
    virtual void run()=0;

protected:
    bool shouldTerminate;
    ConnectionHandler connectionHandler;
};

class clientTask: public Task{
public:
    clientTask(ConnectionHandler *connectionhandler, bool shouldTerminate);
    virtual void run();
};

class serverTask: public Task{
public:
    serverTask(ConnectionHandler *connectionhandler, bool shouldTerminate);
    virtual void run();
};

#endif //CLIENT_TASK_H
