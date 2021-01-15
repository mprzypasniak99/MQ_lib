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
#include "AbstractServerConnection.hpp"


class ServerConnection : public AbstractServerConnection {
    protected:
        int serverSocket; // socket used to contact with server
        std::string ip; // server IP
        std::string port; // server port
        
        bool connectToServer(); // set up connection to server

    public:
        ServerConnection(); // default constructor
        ServerConnection(char* ip, char* port); // could be used to connect to server not from config file

        virtual void setMaxLength(uint16_t maxL) override;

        virtual void setDefaultValidity(uint16_t defT) override;

        virtual int getSocket();

        void logIn(std::string user, std::string pass); // request log in

        void logOut(); // request log out

        void requestRegistration(std::string user, std::string pass); // register new username and password

        void deleteUser(); // delete currently logged in user

        void disconnect(); // disconnect from the server

        void requestQueueList(); // request list of available queues

        void joinQueue(std::string qName);

        void leaveQueue(std::string qName);

        void createQueue(std::string qName, bool isPrivate);

        void deleteQueue(std::string qName);

        void addMessage(std::string qName, std::string contents, uint16_t validityTime);

        void inviteUser(std::string qName, std::string username);
};

#endif