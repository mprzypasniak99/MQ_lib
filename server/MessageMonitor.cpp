#include "MessageMonitor.hpp"

void MessageMonitor::nextMessage() {
    nextToRead = nextToRead->nextMessage;
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