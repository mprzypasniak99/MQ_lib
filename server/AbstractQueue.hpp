#ifndef ABSTRACTQUEUE_HPP
#define ABSTRACTQUEUE_HPP

#include "AbstractMessage.hpp"
#include "ResourceMonitor.hpp"

#include <mutex>


class AbstractQueue {
    protected:
        

    public:
        virtual void updateMonitors(AbstractMessage* delMessage) = 0;
        virtual void updateMonitors() = 0;
        virtual AbstractMessage* getLastMessage() = 0;
        virtual void setLastMessage(AbstractMessage* m) = 0;
        
        ResourceMonitor qMonitor;
};

#endif