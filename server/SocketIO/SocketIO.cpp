#include "SocketIO.hpp"

SocketIO::SocketIO(int fd, ConnectionMonitor* conn) {
    socket = fd;
    connMonitor = conn;
}