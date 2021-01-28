#include "MessageHandler.hpp"

std::string MessageHandler::readToBuffer() {
    uint16_t size;

    read(serverSocket, &size, sizeof(uint16_t));

    char buf[size + 1];
    buf[size] = '\0';

    int ammountRead = 0;

    while(ammountRead < size) {
        ammountRead += read(serverSocket, buf + ammountRead, size - ammountRead);
    }

    return std::string(buf);
}

void MessageHandler::setAddMessageResponse(std::function<void(uint16_t)> response) {
    addMessageResponse = response;
}

void MessageHandler::setCreateQueueResponse(std::function<void(uint16_t)> response) {
    createQueueResponse = response;
}

void MessageHandler::setDeleteQueueResponse(std::function<void(uint16_t)> response) {
    deleteQueueResponse = response;
}

void MessageHandler::setDeleteUserResponse(std::function<void(uint16_t)> response) {
    deleteUserResponse = response;
}

void MessageHandler::setJoinQueueResponse(std::function<void(uint16_t)> response) {
    joinQueueResponse = response;
}

void MessageHandler::setLeaveQueueResponse(std::function<void(uint16_t)> response) {
    leaveQueueResponse = response;
}

void MessageHandler::setLogInResponse(std::function<void(uint16_t)> response) {
    logInResponse = response;
}

void MessageHandler::setLogOutResponse(std::function<void(uint16_t)> response) {
    logOutResponse = response;
}

void MessageHandler::setRegistrationResponse(std::function<void(uint16_t)> response) {
    registrationResponse = response;
}

void MessageHandler::setInviteUserResponse(std::function<void(uint16_t)> response) {
    inviteUserResponse = response;
;}

void MessageHandler::setQueueListRespone(std::function<void(uint16_t, std::vector<std::string>)> response) {
    queueListResponse = response;
}

void MessageHandler::setMessageResponse(std::function<void(std::string, std::string, std::string)> response) {
    receiveMessageResponse = response;
}

void MessageHandler::setUserListResponse(std::function<void(uint16_t, std::vector<std::string>)> response) {
    userListResponse = response;
}

MessageHandler::MessageHandler(AbstractServerConnection* server) {
    this->server = server;
    serverSocket = server->getSocket();

    epoll_fd = epoll_create1(0);

    // create epoll event for handling new connections
    epoll_event ee = {EPOLLIN, {.ptr = this}};

    // add that event to epoll instance
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server->getSocket(), &ee);
}

void MessageHandler::loop() {
    std::thread t = std::thread([&]() {
        epoll_event ee = {}; // structure for event that currently occurs
        while(true){
            if(-1 == epoll_wait(epoll_fd, &ee, 1, -1)) { // wait for events
                error(0,errno,"epoll_wait failed");
            }
        ((Handler*)(ee.data.ptr))->handleEvent(ee.events); // use apropriate method to handle event
        }
    });
    t.detach();
}

void MessageHandler::handleEvent(uint32_t events) {
    if(events & EPOLLIN) { // checks whether event that occured is about possible input
                
        uint16_t choice; // variable storing data read from socket - type of request
                
        // check if read is really possible
        if(read(serverSocket, &choice, sizeof(uint16_t)) > 0) {
                    
            
            if(choice == 1) { // LOG IN
                logInResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if (choice == 2) { // LOG OUT
                logOutResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if (choice == 3) { // REGISTER NEW USER
                registrationResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            } 
            else if (choice == 4) { // DELETE USER
                deleteUserResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if (choice == 6) {// SEND ALL QUEUES
                uint16_t status = readIntToBuffer();
                
                std::string list = readToBuffer();
                unsigned long coma1 = -1, coma2 = list.find(',');

                std::vector<std::string> vec;


                do {
                    vec.push_back(list.substr(coma1 + 1, coma2 - coma1 - 1));
                    coma1 = coma2;
                    coma2 = list.find(',', coma1 + 1);
                    if(coma2 == std::string::npos && coma1 != coma2) {
                        vec.push_back(list.substr(coma1 + 1, coma2 - coma1 - 1));
                    }
                } while(coma2 != std::string::npos);

                queueListResponse(status, vec);
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if (choice == 7) { // JOIN QUEUE
                joinQueueResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if(choice == 8) { // LEAVE QUEUE
                leaveQueueResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if(choice == 9) { // CREATE QUEUE
                createQueueResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if(choice == 10) { // DELETE QUEUE
                deleteQueueResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if(choice == 11) { // SEND MESSAGE TO QUEUE
                addMessageResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if(choice == 12) { // INVITE USER TO QUEUE
                inviteUserResponse(readIntToBuffer());
                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if(choice == 13) { // SERVER PARAMETERS - DEFAULT TIME AND MAX LENGTH
                server->setDefaultValidity(readIntToBuffer());
                server->setMaxLength(readIntToBuffer());

                server->responseMutex.unlock(); // unlock to allow next request to be sent
            }
            else if(choice == 14) { // USERS IN QUEUE LIST
                uint16_t status = readIntToBuffer();
                
                std::string list = readToBuffer();
                unsigned long coma1 = -1, coma2 = list.find(',');

                std::vector<std::string> vec;


                do {
                    vec.push_back(list.substr(coma1 + 1, coma2 - coma1 - 1));
                    coma1 = coma2;
                    coma2 = list.find(',', coma1 + 1);
                    if(coma2 == std::string::npos && coma1 != coma2) {
                        vec.push_back(list.substr(coma1 + 1, coma2 - coma1 - 1));
                    }
                } while(coma2 != std::string::npos);

                userListResponse(status, vec);
                server->responseMutex.unlock();
            }
            else if(choice == 21) { // RECEIVE MESSAGE
                std::string message = readToBuffer();
                int coma1, coma2;
                coma1 = message.find(',');
                coma2 = message.find(',', coma1 + 1);
                uint16_t len = coma1;
                
                receiveMessageResponse(message.substr(0, coma1), // queue name
                message.substr(coma1 + 1, coma2 - coma1 - 1), // user name
                message.substr(coma2 + 1)); // message contents

                server->writeMutex.lock();
                write(serverSocket, &choice, sizeof(uint16_t));
                write(serverSocket, &len, sizeof(uint16_t));
                write(serverSocket, message.substr(0, coma1).c_str(), coma1);
                server->writeMutex.unlock();
            }    
        }        
    }
}

uint16_t MessageHandler::readIntToBuffer() {
    uint16_t buf;

    read(serverSocket, &buf, sizeof(uint16_t));

    return buf;
}