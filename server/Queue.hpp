#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "Message.hpp"
#include <unordered_map>

class MessageMonitor {
    private:
        Message* lastRead;

    public:
        MessageMonitor(Message* m);
        Message* getMessage();

        bool nextMessage(); // Goes to the next(newer) message in the queue if it exists
        // could be deleted and everything could be implemented in getMessage, 
        // since we want to get it only once
};

class Queue {
    private:
        std::string name; // queue name
        
        bool is_private; // whether queue is private or public

        std::string owner; // owner of the queue

        Message* lastMessage; // newest message in the queue

        std::unordered_map<std::string, MessageMonitor*> lastReadMessages; 
        // key - user name, value - lastRead message
    
    public:

        Queue(const char *owner, bool privacy, const char *name);

        void addQueueClient(std::string client, MessageMonitor* monitor);

        /* =========== SETTERS ============= */

        void setName(std::string name);

        void setPrivacy(bool privacy);

        void setOwner(std::string owner);

        void setLastMessage(Message* m);

        /* =========== GETTERS ============= */

        std::string getName();

        bool getPrivacy();

        std::string getOwner();

        Message* getLastMessage();


};


#endif