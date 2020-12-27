#include "../include/Task.h"

Task::Task(ConnectionHandler connectionhandler, bool shouldTerminate):connectionHandler(),shouldTerminate() {
    connectionhandler = connectionhandler;
    shouldTerminate = shouldTerminate;
}
//TODO put in the right way so we will use the same object
clientTask::clientTask(ConnectionHandler *connectionhandler, bool shouldTerminate): Task(connectionhandler, shouldTerminate){}

void clientTask::run() {
    while (!shouldTerminate){
    const short bufsize = 1024;
    //TODO do we need to add an option to double the size of the buffer if needed? if the input isn't valid throw an exception?
    char buf[bufsize];
    std::cin.getline(buf, bufsize);
    std::string line(buf);
    int len=line.length();
    if (!connectionHandler.sendLine(line)) {
        std::cout << "Disconnected. Exiting...\n" << std::endl;
        break;
    }
    // connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
    std::cout << "Sent " << len+1 << " bytes to server" << std::endl;
    }
}

serverTask::serverTask(ConnectionHandler *connectionhandler, bool shouldTerminate): Task(connectionhandler, shouldTerminate){}

void serverTask::run() {
    while(!shouldTerminate) {
        std::string answer;
        // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
        if (!connectionHandler.getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }

        int len = answer.length();
        // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
        // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
        answer.resize(len - 1);
        std::cout << "Reply: " << answer << " " << len << " bytes " << std::endl << std::endl;
        //TODO: change answer bye
        if (answer == "ACK 4") {
            shouldTerminate = true;
            std::cout << "Exiting...\n" << std::endl;
        }
    }
}