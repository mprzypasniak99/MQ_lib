#include "Queue.hpp"

MessageMonitor::MessageMonitor(Message* m){
    this->lastRead = m;
}

Message* MessageMonitor::getMessage(){
    return lastRead;
}

Queue::Queue(std::string owner, bool privacy, std::string name){
    setOwner(owner);
    setPrivacy(privacy);
    setName(name);
    setLastMessage(nullptr);
    addQueueClient(owner, new MessageMonitor(getLastMessage()));
}

void Queue::setName(std::string name){
    this->name = name;
}

void Queue::setPrivacy(bool privacy){
    this->is_private = privacy;
}

void Queue::setOwner(std::string owner){
    this->owner = owner;
}

void Queue::setLastMessage(Message* m){
    this->lastMessage = m;
}

std::string Queue::getName(){
    return name;
}

bool Queue::getPrivacy(){
    return is_private;
}

std::string Queue::getOwner(){
    return owner;
}

Message* Queue::getLastMessage(){
    return lastMessage;
}

void Queue::addQueueClient(std::string client, MessageMonitor* monitor){
    lastReadMessages.insert(std::make_pair(client, monitor));
}