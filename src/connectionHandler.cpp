#include <cstring>
#include <vector>
#include "../include/connectionHandler.h"
 
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(std::string host, short port): host_(host), port_(port), io_service_(),
socket_(io_service_), shouldTerminate(false), isLogged(false), finish(false){}
    
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
    return getFrameAscii(line, '\0');
}

//not using
bool ConnectionHandler::sendLine(std::string& line) {
    return sendFrameAscii(line, '\n');
}
 

bool ConnectionHandler::getFrameAscii(std::string& frame, char delimiter) {
    char ch;
    //OPcode serves as the OPcode but also as a signal
    short OPcode = -1;
    //pass signals the first 4 bytes of the the ACK/ERROR message and it's OPcode to be decoded first
    bool pass = true;
    try {
	do{
        if(!getBytes(&ch, 1)){
			return false;
		}
        //the first 4 bytes of the message handled
		if(pass) {
            decode(frame, ch, OPcode, pass);
        }
		else{
            frame.append(1,ch);
		}
	}while (delimiter != ch || pass);
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
 
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

std::vector<char> ConnectionHandler::encode(char* bytesArr){
    char *token;
    //strtok splits the char to an array of chars and token is a pointer that points to the first char in the array
    token = std::strtok(bytesArr, " ");
    std::string line(token);
    short OPcode;
    // signals if we need to add '\0' to the end of each String
    bool append;
    //the number of words to append to the message after the OPcode
    int counter = 0;
    //OPcode checking:
    if(line == "ADMINREG"){
        OPcode = 1;
        append = true;
        counter = 2;
    }
    else if(line == "STUDENTREG"){
        OPcode = 2;
        append = true;
        counter = 2;
    }
    else if(line == "LOGIN"){
        OPcode = 3;
        append = true;
        counter = 2;
    }
    else if(line == "LOGOUT"){
        OPcode = 4;
        append = false;
        counter = 0;
        //if the user is logged in than Logout will definitely be ACK, than it's time for the Task to terminate, we do
        //this before receiving the ACK 4 message so that the Task won't get back to the cin in the loop which is a
        //blocking method.
        if(isLogged){
            shouldTerminate = true;
        }
    }
    else if(line == "COURSEREG"){
        OPcode = 5;
        append = false;
        counter = 1;
    }
    else if(line == "KDAMCHECK"){
        OPcode = 6;
        append = false;
        counter = 1;
    }
    else if(line == "COURSESTAT"){
        OPcode = 7;
        append = false;
        counter = 1;
    }
    else if(line == "STUDENTSTAT"){
        OPcode = 8;
        append = true;
        counter = 1;
    }
    else if(line == "ISREGISTERED"){
        OPcode = 9;
        append = false;
        counter = 1;
    }
    else if(line == "UNREGISTER"){
        OPcode = 10;
        append = false;
        counter = 1;
    }
    else if(line == "MYCOURSES"){
        OPcode = 11;
        append = false;
        counter = 0;
    }
    //if none of the words above are written than it's an error
    else{
        std::vector<char> vec (0);
        return vec;
    }
    //putting the OPcode bytes in output
    std::vector<char> output(0);
    output.push_back(((OPcode >> 8) & 0xFF));
    output.push_back((OPcode & 0xFF));
    //we need to add '\0' in the end of each word
    if(append){
        int i = 0;
        while(i < counter) {
            //gets the next char in the array of chars
            token = strtok(NULL," \0");
            output.insert(output.end(), token, token + strlen(token));
            output.push_back('\0');
            i ++;
        }
    }
    //we don't
    else if(counter != 0){
        //gets the next char in the array of chars
        token = strtok(NULL," \0");
        //the char is courseNum
        short course = (short) strtoul(token, NULL, 0);
        output.push_back(((course >> 8) & 0xFF));
        output.push_back((course & 0xFF));
    }
    return output;
}

void ConnectionHandler::decode(std::string& frame, char ch, short& OPcode, bool& pass) {
    //first byte
    if(OPcode == -1){
        OPcode = (short)((ch & 0xff) << 8);
    }
    //second byte
    else if(OPcode == 0){
        OPcode += (short)(ch & 0xff);
        if(OPcode == 12){
            frame.append("ACK");
        }
        else{
            frame.append("ERROR");
        }
        // signals the next byte is the third byte
        OPcode = 14;
    }
    //the third byte
    else if(OPcode == 14){
        frame.append(" ");
        // signals the next byte is the fourth byte
        OPcode = 15;
    }
        // signals the next byte is the fourth byte
    else{
        OPcode = (short)(ch & 0xff);
        frame.append(std::to_string(OPcode));
        //if we got ACK 3 than the user is logged in now
        if(frame == "ACK 3"){
            isLogged = true;
        }
        //signals we finished the handle of the first 4 bytes
        OPcode = -1;
        pass = false;
    }
}

bool ConnectionHandler::getTerminate() {return shouldTerminate;}

bool ConnectionHandler::getFinish() {return finish;}

void ConnectionHandler::setFinish() {finish = true;}

