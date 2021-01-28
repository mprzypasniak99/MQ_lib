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
#include <sys/epoll.h>
#include <thread>
#include "Handler.hpp"
#include "AbstractServerConnection.hpp"

class MessageHandler: public Handler {
    private:
        AbstractServerConnection* server; // socket to communicate with server

        int serverSocket; // server socket

        int epoll_fd; // file descriptor for epoll event loop

        // functions handling response to requests received from server
        std::function<void(uint16_t)> logInResponse;
        std::function<void(uint16_t)> logOutResponse;
        std::function<void(uint16_t)> registrationResponse;
        std::function<void(uint16_t)> deleteUserResponse;
        std::function<void(uint16_t, std::vector<std::string>)> queueListResponse;
        std::function<void(uint16_t)> joinQueueResponse;
        std::function<void(uint16_t)> leaveQueueResponse;
        std::function<void(uint16_t)> createQueueResponse;
        std::function<void(uint16_t)> deleteQueueResponse;
        std::function<void(uint16_t)> addMessageResponse;
        std::function<void(uint16_t)> inviteUserResponse;
        
        std::function<void(uint16_t, std::vector<std::string>)> userListResponse;
        std::function<void(std::string, std::string, std::string)> receiveMessageResponse;
        // arguments order: queue, user, content

        std::string readToBuffer(); // function for reading size and that number of bytes from socket

        uint16_t readIntToBuffer(); // function for reading integer from socket
    public:
        MessageHandler(AbstractServerConnection* server);

        void setLogInResponse(std::function<void(uint16_t)> response);
        void setLogOutResponse(std::function<void(uint16_t)> response);
        void setRegistrationResponse(std::function<void(uint16_t)> response);
        void setDeleteUserResponse(std::function<void(uint16_t)> response);
        void setJoinQueueResponse(std::function<void(uint16_t)> response);
        void setLeaveQueueResponse(std::function<void(uint16_t)> response);
        void setCreateQueueResponse(std::function<void(uint16_t)> response);
        void setDeleteQueueResponse(std::function<void(uint16_t)> response);
        void setAddMessageResponse(std::function<void(uint16_t)> response);
        void setInviteUserResponse(std::function<void(uint16_t)> response);
        void setUserListResponse(std::function<void(uint16_t, std::vector<std::string>)> response);

        void setQueueListRespone(std::function<void(uint16_t, std::vector<std::string>)> response);
        
        void setMessageResponse(std::function<void(std::string, std::string, std::string)> response);

        void loop(); // main loop for receiving responses from server

        virtual void handleEvent(uint32_t events) override; 
        // method recognizing type of response and calling valid user functions
};

#endif