/*
Project uses rapidjson library for parsing json files
Github: https://github.com/Tencent/rapidjson
*/

#include "MQ_lib.hpp"

/* =============== DEFAULT CONTRUCTOR ================== */
ServerConnection::ServerConnection() {
    int fd = open("config/connection.json", O_RDONLY); // open config file and handle errors
    if(fd == -1) {
        error(-1, errno, "Failed to open config file\n");
    }
    char buf[256]; // buffer for reading file
    int count = 0;
    std::string file = "";

    // read whole file
    while( (count = read(fd, buf, 256)) > 0){
        buf[count] = '\0';
        file += buf;
    }

    close(fd); // close config file

    // parse json
    rapidjson::Document d;
    d.Parse(file.c_str());

    // read needed fields
    ip = d["IP"].GetString();
    port = d["port"].GetString();

    // connect to server and indicate possible errors errors
    if(!connectToServer()) {
        error(-1, errno, "Failed to connect to server\n");
    }
}

/* ============== NON-STANDARD CONSTRUCTOR ================ */
ServerConnection::ServerConnection(char* ip, char* port) {
    // save received ip and adress
    this->ip = ip;
    this->port = port;

    // connect to server
    if(!connectToServer()) {
        error(-1, errno, "Failed to connect to server\n");
    }
}

// function used to set up connection to server
bool ServerConnection::connectToServer() {
    responseMutex.lock();

    addrinfo hints = {};
    // point what address and protocol you'll be using to connect
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo *result;

    // get server IP address
    if(int err = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result)) {
        error(-1, 0, "Resolving address failed: %s", gai_strerror(err));
        return(false);
    }

    // create socket with read data
    this->serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    // try to connect and throw error if it occurs
    if(connect(this->serverSocket, result->ai_addr, result->ai_addrlen)){
        error(-1, errno, "Failed to connect to %s:%s", this->ip.c_str(), this->port.c_str());
        return(false);
    }
    freeaddrinfo(result); // delete addrinfo structure

    return true;
}

// send request for logging in
void ServerConnection::logIn(std::string user, std::string pass) {
    
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object

    uint16_t action = 1; // request type

    // send information about kind of request you're about to make
    write(serverSocket, &action, sizeof(uint16_t)); 
    
    // prepare input string
    std::string input = user;
    input += ","; input += pass;

    uint16_t length = input.length();


    write(serverSocket, &length, sizeof(uint16_t)); // send message length
    
    write(serverSocket, input.c_str(), input.length()); // send request message
}

void ServerConnection::logOut() {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object

    uint16_t action = 2; // request number

    // success in sending request is treated as success in logging out
    write(serverSocket, &action, sizeof(uint16_t));    
}

void ServerConnection::requestRegistration(std::string user, std::string pass) {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object

    uint16_t action = 3; // request type

    // send information about kind of request you're about to make
    write(serverSocket, &action, sizeof(uint16_t)); 
    
    // prepare input string
    std::string input = user;
    input += ","; input += pass;

    uint16_t length = input.length();


    write(serverSocket, &length, sizeof(uint16_t)); // send message length
    
    write(serverSocket, input.c_str(), input.length()); // send request message
}

void ServerConnection::deleteUser() {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object

    uint16_t action = 4;

    write(serverSocket, &action, sizeof(uint16_t));

}

void ServerConnection::disconnect() {
    responseMutex.lock(); // will be unlocked after sending request since there's no response to that request

    uint16_t action = 5;

    if(write(serverSocket, &action, sizeof(uint16_t)) > 0) {
        shutdown(serverSocket, SHUT_RDWR); // disconnect
        close(serverSocket); // close file descriptor
    }

    responseMutex.unlock();
}

void ServerConnection::requestQueueList() {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object

    uint16_t action = 6;

    write(serverSocket, &action, sizeof(uint16_t));
}

void ServerConnection::joinQueue(std::string qName) {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object
    
    uint16_t action = 7;
    uint16_t size = qName.size();

    write(serverSocket, &action, sizeof(uint16_t));
    write(serverSocket, &size, sizeof(uint16_t));
    write(serverSocket, qName.c_str(), size);

}

void ServerConnection::leaveQueue(std::string qName) {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object
    
    uint16_t action = 8;
    uint16_t size = qName.size();

    write(serverSocket, &action, sizeof(uint16_t));
    write(serverSocket, &size, sizeof(uint16_t));
    write(serverSocket, qName.c_str(), size);
}

void ServerConnection::createQueue(std::string qName, bool isPrivate) {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object
    
    uint16_t action = 9;
    uint16_t size = qName.size();

    write(serverSocket, &action, sizeof(uint16_t));
    write(serverSocket, &size, sizeof(uint16_t));
    write(serverSocket, qName.c_str(), size);
    write(serverSocket, &isPrivate, sizeof(bool));
}

void ServerConnection::deleteQueue(std::string qName) {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object
    
    uint16_t action = 10;
    uint16_t size = qName.size();

    write(serverSocket, &action, sizeof(uint16_t));
    write(serverSocket, &size, sizeof(uint16_t));
    write(serverSocket, qName.c_str(), size);
}

void ServerConnection::addMessage(std::string qName, std::string contents, uint16_t validityTime) {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object
    
    uint16_t action = 11;

    uint16_t size = qName.size();

    write(serverSocket, &action, sizeof(uint16_t));
    write(serverSocket, &size, sizeof(uint16_t));
    write(serverSocket, qName.c_str(), size);

    size = contents.size();

    write(serverSocket, &size, sizeof(uint16_t));
    write(serverSocket, contents.c_str(), size);

    write(serverSocket, &validityTime, sizeof(uint16_t));
}

void ServerConnection::inviteUser(std::string qName, std::string username) {
    responseMutex.lock(); // will be unlocked upon receiving response by MessageHandler object
    
    uint16_t action = 12;

    uint16_t size = qName.size();

    write(serverSocket, &action, sizeof(uint16_t));
    write(serverSocket, &size, sizeof(uint16_t));
    write(serverSocket, qName.c_str(), size);

    size = username.size();

    write(serverSocket, &size, sizeof(uint16_t));
    write(serverSocket, username.c_str(), size);
}

void ServerConnection::setMaxLength(uint16_t maxL) {
    maxLength = maxL;
}

void ServerConnection::setDefaultValidity(uint16_t defT) {
    defaultTime = defT;
}

int ServerConnection::getSocket() {
    return serverSocket;
}