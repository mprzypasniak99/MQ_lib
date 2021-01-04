#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "Message.hpp"
#include <unordered_map>

class MessageMonitor {
    private:
        Message* lastRead;

    public:
        Message* getMessage();

        void nextMessage();
};

class Queue {
    private:
        std::string name; // queue name
        
        bool is_private; // whether queue is private or public

        std::string owner; // owner of the queue

        Message* lastMessage; // newest message in the queue

        std::unordered_map<std::string, MessageMonitor*> lastReadMessages; 
        // key - user name, value - lastRead message
};


#endif