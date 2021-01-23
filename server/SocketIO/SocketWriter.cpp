#include "SocketWriter.hpp"

SocketWriter::SocketWriter(int fd, ConnectionMonitor* conn) : SocketIO(fd, conn) {}

int SocketWriter::writeInt(uint16_t val) {
    int result;
    try{
    while (write(socket, &val, sizeof(val)) <= 0) {
        result = timeout();
        if(result <= 0) return result;
    }
    } catch(...) {
        std::current_exception();
    }
    
    connMonitor->updateWrite();
    connMonitor->confirmMessage();
    return 1;
}

int SocketWriter::writeString(std::string val) {
    int alreadyWriten = 0, thisTime = 0;
    int len = val.length();
    int result;
    while(alreadyWriten < len) {
        thisTime = write(socket, val.c_str() + alreadyWriten, len - alreadyWriten);
        if(thisTime == 0) {
            result = timeout();
            if(result <= 0) return result;
        }
        else {
            alreadyWriten += thisTime;
        }
    }

    connMonitor->updateWrite();
    connMonitor->confirmMessage();
    return 1;
}