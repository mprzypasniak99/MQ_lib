#include "SocketReader.hpp"

SocketReader::SocketReader(int fd, ConnectionMonitor* conn) : SocketIO(fd, conn) {}

bool SocketReader::readUint16(uint16_t* val) {
    while(read(socket, val, sizeof(uint16_t)) <= 0 ) {
        if(connMonitor->timeout()) {
            return false;
        }
    }

    connMonitor->updateRead();
    return true;
}

bool SocketReader::readString(std::string* val, int len) {
    int alreadyRead = 0, thisTime = 0;
    char buf[len+1];
    buf[len] = '\0';


    while(alreadyRead < len) {
        thisTime = read(socket, buf + alreadyRead, len - alreadyRead);
        if(thisTime <= 0) {
            if(connMonitor->timeout()) {
                return false;
            }
        }
        else {
            alreadyRead += thisTime;
        }
    }

    *val = buf;

    connMonitor->updateRead();
    return true;
}

bool SocketReader::readBool(bool* val) {

    while(read(socket, val, sizeof(bool)) <= 0 ) {
        if(connMonitor->timeout()) {
            return false;
        }
    }

    connMonitor->updateRead();
    return true;
}