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
        
        virtual void updateMonitors(AbstractMessage* delMessage); // for deletion purposes

        virtual void updateMonitors(); // for adding new message
        
        Queue(const char *owner, bool privacy, const char *name);

        bool addQueueClient(std::string client);

        bool deleteQueueClient(std::string client);
        
        void addMessage(Message*);

        /* =========== SETTERS ============= */

        void setName(std::string name);

        void setPrivacy(bool privacy);

        void setOwner(std::string owner);

        virtual void setLastMessage(AbstractMessage* m);

        /* =========== GETTERS ============= */

        std::string getName();

        bool getPrivacy();

        std::string getOwner();

        Message* getLastMessage();

        MessageMonitor* getUserMonitor(std::string name);


};


#endif