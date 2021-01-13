#ifndef MESSAGEMONITOR_HPP
#define MESSAGEMONITOR_HPP

#include "Message.hpp"

class MessageMonitor {
    private:
        Message* nextToRead; // next message to be read by user

    public:
        MessageMonitor(Message* m);
        Message* getMessage();

        void nextMessage(); // Goes to the next(newer) message in the queue if it exists
        
        void setMessage(Message *next);
};

#endif