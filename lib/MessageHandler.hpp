#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <functional>

class MessageHandler {
    private:
        int serverSocket; // socket to communicate with server

        // functions handling response to requests received from server
        std::function<void(uint16_t)> logInResponse;
        std::function<void(uint16_t)> logOutResponse;
        std::function<void(uint16_t)> registrationResponse;
        std::function<void(uint16_t)> deleteUserResponse;
    
    public:
        void setLogInResponse(std::function<void(uint16_t)> response);
        void setLogOutResponse(std::function<void(uint16_t)> response);
        void setRegistrationResponse(std::function<void(uint16_t)> response);
        void setDeleteUserResponse(std::function<void(uint16_t)> response);
};

#endif