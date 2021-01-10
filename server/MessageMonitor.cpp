#include "MessageMonitor.hpp"

bool MessageMonitor::previousMessage() {
    
}

MessageMonitor::MessageMonitor(Message* m){
    this->lastRead = m;
}

Message* MessageMonitor::getMessage(){
    return lastRead;
}