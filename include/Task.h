#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

#include "../include/connectionHandler.h"

class Task{
public:
    virtual void run()=0;
};

class clientTask: public Task{
public:
    virtual void run();
};

class serverTask: public Task{
public:
    virtual void run();
};

#endif //CLIENT_TASK_H
