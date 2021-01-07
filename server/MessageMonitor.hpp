#ifndef MESSAGEMONITOR_HPP
#define MESSAGEMONITOR_HPP

#include "Message.hpp"

class MessageMonitor {
    private:
        Message* lastRead;

    public:
        MessageMonitor(Message* m);
        Message* getMessage();

        bool previousMessage(); // Goes to the previous(older) message in the queue if it exists
        
};

#endif