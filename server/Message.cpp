#include "Message.hpp"


Message::Message(const char* sender, long time, const char* message, Message* last) {
    this->sender = sender;
    this->contents = message;
    this->timer = Timer();
    timer.setTimeout([this](){this->handleTimer(this->timer);}, time); //In seconds or not in seconds, that is the question: explaining, time is in seconds or not, because function takes miliseconds I think
    setPrevMessage(last);
    setNextMessage(nullptr);
    last->setNextMessage(this);
}

Message::~Message() {
    queue->updateMonitors(this);
}

void Message::setNextMessage(Message* m){
    this->nextMessage = m;
}

void Message::setPrevMessage(Message* m){
    this->previousMessage = m;
}

void Message::handleTimer(Timer t){
    t.stop();
    delete this;

}

std::string Message::getContents() {
    return contents;
}

std::string Message::getSender() {
    return sender;
}