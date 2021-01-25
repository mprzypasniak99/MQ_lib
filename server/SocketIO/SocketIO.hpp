#ifndef SOCKETIO_HPP
#define SOCKETIO_HPP

#include "../ConnectionMonitor.hpp"

class SocketIO
{
protected:
    int socket;
    ConnectionMonitor* connMonitor;

public:
    SocketIO(int fd, ConnectionMonitor* conn);
    ~SocketIO() = default;
};

#endif