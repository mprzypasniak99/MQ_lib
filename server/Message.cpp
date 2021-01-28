#include "Message.hpp"


Message::Message(std::string sender, long time, std::string message, AbstractQueue *q) {
    this->sender = sender;
    this->contents = message;
    this->timer = Timer();
    this->queue = q;
    setPrevMessage(nullptr);
    setNextMessage(nullptr);
    timer.setTimeout([this](){this->handleTimer();}, time); //In seconds or not in seconds, that is the question: explaining, time is in seconds or not, because function takes miliseconds I think
}

Message::~Message() {
    queue->qMonitor.enterWrite(); // block modification of the queue structure

    queue->updateMonitors(this);
    if(this->previousMessage != nullptr) this->previousMessage->nextMessage = this->nextMessage;
    if(this->nextMessage != nullptr) this->nextMessage->previousMessage = this->previousMessage;
    if(this->queue->getLastMessage() == this) this->queue->setLastMessage(this->previousMessage);
    
    queue->qMonitor.exitWrite();
}

void Message::setNextMessage(Message* m){
    this->nextMessage = m;
}

void Message::setPrevMessage(Message* m){
    this->previousMessage = m;
}

void Message::handleTimer(){
    timer.stop();
    delete this;

}

std::string Message::getContents() {
    return contents;
}

std::string Message::getSender() {
    return sender;
}