#ifndef ABSTRACTQUEUE_HPP
#define ABSTRACTQUEUE_HPP


class AbstractQueue {
    public:
        virtual void updateMonitors(void *delMessage) = 0;
};

#endif