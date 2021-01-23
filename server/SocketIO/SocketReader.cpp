#include "SocketReader.hpp"

SocketReader::SocketReader(int fd, ConnectionMonitor* conn) : SocketIO(fd, conn) {}

int SocketReader::readUint16(uint16_t* val) {
    int result;
    while(read(socket, val, sizeof(uint16_t)) <= 0 ) {
        result = timeout();
        if(result <= 0) return result;
    }

    connMonitor->updateRead();
    connMonitor->confirmMessage();
    return 1;
}

int SocketReader::readString(std::string* val, int len) {
    int alreadyRead = 0, thisTime = 0;
    char buf[len+1];
    buf[len] = '\0';

    int result;

    while(alreadyRead < len) {
        thisTime = read(socket, buf + alreadyRead, len - alreadyRead);
        if(thisTime == 0) {
            result = timeout();
            if(result <= 0) return result;
        }
        else {
            alreadyRead += thisTime;
        }
    }

    *val = buf;

    connMonitor->updateRead();
    connMonitor->confirmMessage();
    return 1;
}

int SocketReader::readBool(bool* val) {
    int result;

    while(read(socket, val, sizeof(bool)) <= 0 ) {
        result = timeout();
        if(result <= 0) return result;
    }

    connMonitor->updateRead();
    connMonitor->confirmMessage();
    return 1;
}