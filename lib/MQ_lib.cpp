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
    
    std::string file = "";

    // read whole file
    while(read(fd, buf, 256) > 0){
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
    this->server_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    // try to connect and throw error if it occurs
    if(connect(this->server_socket, result->ai_addr, result->ai_addrlen)){
        error(-1, errno, "Failed to connect to %s:%s", this->ip.c_str(), this->port.c_str());
        return(false);
    }
    freeaddrinfo(result); // delete addrinfo structure

    return true;
}

// send request for logging in
bool ServerConnection::logIn(const char *user, const char* pass) {
    
    uint16_t action = htons(1); // convert request number to network byte order

    // send information about kind of request you're about to make
    write(server_socket, &action, sizeof(uint16_t)); 
    
    // prepare input string
    std::string input = user;
    input += ","; input += pass; input += ";";

    uint16_t length = input.length();
    length = htons(length); // convert message lenght to network byte order



    write(server_socket, &length, sizeof(uint16_t)); // send message length
    
    write(server_socket, input.c_str(), input.length()); // send request message

    uint16_t status;

    read(server_socket, &status, sizeof(uint16_t)); // receive request status - 200 means success

    status = ntohs(status); // convert to host byte order

    // inform about status of request
    if(status == 200) return true;
    else return false;
}

bool ServerConnection::logOut() {
    uint16_t action = htons(2); // request number

    // success in sending request is treated as success in logging out
    if(write(server_socket, &action, sizeof(uint16_t)) > 0) { 
        shutdown(server_socket, SHUT_RDWR); // disconnect
        close(server_socket); // close file descriptor
        return true;
    }
    else {
        return false;
    }
}