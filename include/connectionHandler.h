#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__
                                           
#include <string>
#include <iostream>
#include <mutex>
#include "boost/asio.hpp"

using boost::asio::ip::tcp;

class ConnectionHandler {
private:
    const std::string host_;
    const short port_;
    boost::asio::io_service io_service_;
    tcp::socket socket_;
    //signals Task to terminate
    bool shouldTerminate;
    //signals that the client is logged to the server
    bool isLogged;
    //signals the BGRSclient to finish it's run
    bool finish;

public:
    ConnectionHandler(std::string host, short port);


    virtual ~ConnectionHandler();

    bool connect();

    bool getBytes(char bytes[], unsigned int bytesToRead);

    bool sendBytes(const char bytes[], int bytesToWrite);

    bool getLine(std::string &line);

    bool sendLine(std::string &line);

    bool getFrameAscii(std::string &frame, char delimiter);

    bool sendFrameAscii(const std::string &frame, char delimiter);

    std::vector<char> encode(char* bytesArr);

    void decode(std::string& frame, char ch, short& OPcode, bool& pass);

    bool getTerminate();

    void close();

    bool getFinish();

    //sets the finish to true
    void setFinish();
};
 
#endif