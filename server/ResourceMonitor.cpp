#include "ResourceMonitor.hpp"

ResourceMonitor::ResourceMonitor() {
    read = 0;
    write = 0;
}

void ResourceMonitor::enterRead() {
    tryEnter.lock(); // try to get into reading section
    rmutex.lock();
    read++;
    if(read == 1) resource.lock(); // if it's the first thread trying to read, block out the modifying ones
    rmutex.unlock();
    tryEnter.unlock();
}

void ResourceMonitor::exitRead() {
    rmutex.lock();
    read--;
    if(read == 0) resource.unlock();
    rmutex.unlock();
}

void ResourceMonitor::enterWrite() {
    wmutex.lock();
    write++;
    if(write == 1) tryEnter.lock();
    wmutex.unlock();

    resource.lock();
}

void ResourceMonitor::exitWrite() {
    resource.unlock();
    
    wmutex.lock();
    write--;
    if(write == 0) tryEnter.unlock();
    wmutex.unlock();
}