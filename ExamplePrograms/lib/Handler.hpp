#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <cstdint>

// Abstract class used for handling different kinds of events
class Handler {
    public:
        virtual void handleEvent(uint32_t events) = 0;
        virtual ~Handler() = default;
};

#endif