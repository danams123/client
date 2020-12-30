
#include "../include/connectionHandler.h"
 
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(string host, short port): host_(host), port_(port), io_service_(), socket_(io_service_){}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
			tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);			
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getLine(std::string& line) {
    return getFrameAscii(line, '\n');
}

bool ConnectionHandler::sendLine(std::string& line) {
    return sendFrameAscii(line, '\n');
}
 

bool ConnectionHandler::getFrameAscii(std::string& frame, char delimiter) {
    char ch;
    // Stop when we encounter the null character.
    // Notice that the null character is not appended to the frame string.
    try {
	do{
		if(!getBytes(&ch, 1))
		{
			return false;
		}
		if(ch!='\0')  
			frame.append(1, ch);
		else{
		    frame.append(1,'\n');
		}
	}while (delimiter != ch);
    } catch (std::exception& e) {
	std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
	return false;
    }
    return true;
}
 
 
bool ConnectionHandler::sendFrameAscii(const std::string& frame, char delimiter) {
	bool result=sendBytes(frame.c_str(),frame.length());
	if(!result) return false;
	return sendBytes(&delimiter,1);
}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

char* ConnectionHandler::decode(char* bytesArr){
    const char delim[2] = " ";
    char *token;//possible memory leak
    token = strtok(bytesArr, delim);
    std::string line(token);//check if that works
    short OPcode;
    bool append;
    if(line == "ADMINREG"){
        OPcode = 1;
        append = true;
    }
    else if(line == "STUDENTREG"){
        OPcode = 2;
        append = true;
    }
    else if(line == "LOGOUT"){
        OPcode = 3;
        append = true;
    }
    else if(line == "LOGOUT"){
        OPcode = 4;
        append = false;
    }
    else if(line == "COURSEREG"){
        OPcode = 5;
        append = false;
    }
    else if(line == "KDAMCHECK"){
        OPcode = 6;
        append = false;
    }
    else if(line == "COURSESTAT"){
        OPcode = 7;
        append = true;
    }
    else if(line == "STUDENTSTAT"){
        OPcode = 8;
        append = false;

    }
    else if(line == "ISREGISTERED"){
        OPcode = 9;
        append = false;
    }
    else if(line == "UNREGISTER"){
        OPcode = 10;
        append = false;
    }
    else{
        OPcode = 11;
        append = false;
    }
    char* output = new char(1024);
    output[0] = ((OPcode >> 8) & 0xFF);
    output[1] = (OPcode & 0xFF);
    while(token != NULL) {
        token = strtok(NULL," ");//check if it work!
        strcat(output, token);
        if(append) {
            output[strlen(output) + 1] = '\0';
        }
    }
    return output;
}