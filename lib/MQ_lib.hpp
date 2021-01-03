/*
Project uses rapidjson library for parsing json files
Github: https://github.com/Tencent/rapidjson
*/

#ifndef MQLIB_HPP
#define MQLIB_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include "../rapidjson/document.h"

class ServerConnection {
    private:
        int server_socket; // socket used to contact with server
        std::string ip; // server IP
        std::string port; // server port
        
        bool connectToServer(); // set up connection to server

    public:
        ServerConnection(); // default constructor
        ServerConnection(char* ip, char* port); // could be used to connect to server not from config file

        bool logIn(const char* user, const char* pass); // request log in

        bool logOut(); // request log out
};

#endif