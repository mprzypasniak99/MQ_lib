#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "AbstractQueue.hpp"
#include "MessageMonitor.hpp"
#include <unordered_map>


class Queue : public AbstractQueue{
    protected:
        std::string name; // queue name
        
        bool is_private; // whether queue is private or public

        std::string owner; // owner of the queue

        Message* lastMessage; // newest message in the queue

        std::unordered_map<std::string, MessageMonitor*> lastReadMessages; 
        // key - user name, value - lastRead message

        
    public:
        
        virtual void updateMonitors(void *delMessage);
        
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