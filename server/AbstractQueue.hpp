#ifndef ABSTRACTQUEUE_HPP
#define ABSTRACTQUEUE_HPP

#include "AbstractMessage.hpp"


class AbstractQueue {
    public:
        virtual void updateMonitors(AbstractMessage* delMessage) = 0;
        virtual AbstractMessage* getLastMessage() = 0;
        virtual void setLastMessage(AbstractMessage* m) = 0;
};

#endif