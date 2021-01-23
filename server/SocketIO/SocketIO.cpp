#include "SocketIO.hpp"

SocketIO::SocketIO(int fd, ConnectionMonitor* conn) {
    socket = fd;
    connMonitor = conn;
}

int SocketIO::timeout() {
    bool isFirst;
    if(connMonitor->timeout(isFirst)) {
        if(isFirst) {
            return -1;
        }
        else {
            return 0;
        }
    }
    else return 1;
}