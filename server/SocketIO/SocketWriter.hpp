#ifndef SOCKETWRITER_HPP
#define SOCKETWRITER_HPP

#include <cstdint>
#include <string>
#include <unistd.h>
#include "SocketIO.hpp"

class SocketWriter : public SocketIO
{
public:
    bool writeInt(uint16_t val);
    bool writeString(std::string val);
    SocketWriter(int fd, ConnectionMonitor* conn);
    ~SocketWriter() = default;
};

#endif