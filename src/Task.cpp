#include "../include/Task.h"


void clientTask::run() {
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


void serverTask::run() {
    std::string answer;
    // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
    // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
    if (!connectionHandler.getLine(answer)) {
        std::cout << "Disconnected. Exiting...\n" << std::endl;
        break;
    }

    len=answer.length();
    // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
    // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
    answer.resize(len-1);
    std::cout << "Reply: " << answer << " " << len << " bytes " << std::endl << std::endl;
    if (answer == "bye") {
        std::cout << "Exiting...\n" << std::endl;
        break;
    }
}