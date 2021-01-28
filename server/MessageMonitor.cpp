#include "MessageMonitor.hpp"

void MessageMonitor::nextMessage() {
    if(nextToRead != nullptr) {
        nextToRead = nextToRead->nextMessage;
    }
}

MessageMonitor::MessageMonitor(Message* m){
    nextToRead = m;
}

Message* MessageMonitor::getMessage(){
    return nextToRead;
}

void MessageMonitor::setMessage(Message *next) {
    nextToRead = next;
}