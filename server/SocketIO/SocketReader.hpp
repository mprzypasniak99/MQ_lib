#ifndef SOCKETREADER_HPP
#define SOCKETREADER_HPP

#include <string>
#include <cstdint>
#include <unistd.h>
#include "SocketIO.hpp"

class SocketReader : public SocketIO
{
public:
    int readUint16(uint16_t* val);
    int readString(std::string* val, int len);
    int readBool(bool* val);

    SocketReader(int fd, ConnectionMonitor* conn);
    ~SocketReader() = default;
};


#endif