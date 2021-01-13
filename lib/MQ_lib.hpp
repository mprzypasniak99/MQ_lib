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
#include <thread>

class ServerConnection {
    private:
        int server_socket; // socket used to contact with server
        std::string ip; // server IP
        std::string port; // server port
        
        bool connectToServer(); // set up connection to server

    public:
        ServerConnection(); // default constructor
        ServerConnection(char* ip, char* port); // could be used to connect to server not from config file

        bool logIn(std::string user, std::string pass); // request log in

        bool logOut(); // request log out

        bool requestRegistration(std::string user, std::string pass); // register new username and password

        bool deleteUser(); // delete currently logged in user

        bool disconnect(); // disconnect from the server

        bool requestQueueList(); // request list of available queues

        bool joinQueue(std::string qName);

        bool leaveQueue(std::string qName);

        bool createQueue(std::string qName, bool isPrivate);

        bool deleteQueue(std::string qName);

        bool addMessage(std::string qName, std::string contents, uint16_t validityTime);

        bool inviteUser(std::string qName, std::string username);
};

#endif