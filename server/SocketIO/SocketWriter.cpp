#include "SocketWriter.hpp"

SocketWriter::SocketWriter(int fd, ConnectionMonitor* conn) : SocketIO(fd, conn) {}

bool SocketWriter::writeInt(uint16_t val) {
    if(connMonitor->getIsTimedOut()) {
        return false;
    }
    else {
        while (write(socket, &val, sizeof(val)) <= 0) {
            if(connMonitor->timeout()) {
                return false;
            }
        }
    }
    
    connMonitor->updateWrite();
    return true;
}

bool SocketWriter::writeString(std::string val) {
    if(connMonitor->getIsTimedOut()) {
        return false;
    }
    else
    {
        int alreadyWriten = 0, thisTime = 0;
        int len = val.length();

        while(alreadyWriten < len) {
            thisTime = write(socket, val.c_str() + alreadyWriten, len - alreadyWriten);
            if(thisTime <= 0) {
                if(connMonitor->timeout()) {
                    return false;
                }
            }
            else {
                alreadyWriten += thisTime;
            }
        }
    }
    
    connMonitor->updateWrite();
    return true;
}