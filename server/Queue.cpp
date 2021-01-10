#include "Queue.hpp"

Queue::Queue(const char *owner, bool privacy, const char *name){
    setOwner(owner);
    setPrivacy(privacy);
    setName(name);
    setLastMessage(nullptr);
    addQueueClient(owner);
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

void Queue::setLastMessage(AbstractMessage* m){
    this->lastMessage = (Message*)m;
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

bool Queue::addQueueClient(std::string client){
    Message *tmp = getLastMessage();
    if(tmp != nullptr){
        while(tmp->previousMessage != nullptr) tmp = tmp->previousMessage;
    }
    if(lastReadMessages.find(client) != lastReadMessages.end())
        lastReadMessages.insert(std::make_pair(client, new MessageMonitor(tmp)));
        return true;
    return false;
}

void Queue::updateMonitors(AbstractMessage* delMessage) {
    for(auto i : lastReadMessages ) {
        if(i.second->getMessage() == (Message*)delMessage) {
            i.second->previousMessage();
        }
    }
}

void Queue::addMessage(Message* m){
    Message* tmp = this->getLastMessage();
    tmp->setNextMessage(m);
    m->setPrevMessage(tmp);
    setLastMessage(m);
}