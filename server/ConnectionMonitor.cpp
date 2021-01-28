#include "ConnectionMonitor.hpp"

ConnectionMonitor::ConnectionMonitor() {
    updateRead();
    updateWrite();
    isTimedOut = false;
    currentThreads = 0;
}

void ConnectionMonitor::updateRead() {
    mtx.lock();
    lastRead = std::chrono::system_clock::now();
    mtx.unlock();
}

void ConnectionMonitor::updateWrite() {
    mtx.lock();
    lastWrite = std::chrono::system_clock::now();
    mtx.unlock();
}

bool ConnectionMonitor::timeout() {

    int timeOutVal = 6000; // timeout - wait for response for one minute
    mtx.lock();
    if(!isTimedOut){
        auto stop = std::chrono::system_clock::now();

        auto timeSinceWrite = std::chrono::duration_cast<std::chrono::milliseconds>(stop - lastWrite);
        auto timeSinceRead = std::chrono::duration_cast<std::chrono::milliseconds>(stop - lastRead);
            
        /*  check how much time has passed since last successful
            read/write */
        if( timeSinceRead.count() > timeOutVal
            && timeSinceWrite.count() > timeOutVal) {
            isTimedOut = true;
        }
    }
    mtx.unlock();

    return isTimedOut;
}

bool ConnectionMonitor::getIsTimedOut() {
    mtx.lock();
    bool result = isTimedOut;
    mtx.unlock();
    
    return result;
}

bool ConnectionMonitor::increaseNoThreads() {
    if(!isTimedOut) {
        mtx.lock();
        currentThreads++;
        mtx.unlock();
        
        return true;
    }
    else {
        return false;
    }
    
}

bool ConnectionMonitor::decreaseNoThreads() {
    mtx.lock();
    currentThreads--;
    mtx.unlock();
        
    return true;
}

int ConnectionMonitor::getNoThreads() {
    mtx.lock();
    int result = currentThreads;
    mtx.unlock();

    return result;
}

void ConnectionMonitor::initiateTimeout() {
    mtx.lock();
    isTimedOut = true;
    mtx.unlock();
}

bool ConnectionMonitor::initiateShutdown() {
    bool result;

    mtx.lock();
    if(isTimedOut && !shuttingDown) {
        shuttingDown = true;
        result = true;
    }
    else {
        result = false;
    }
    mtx.unlock();

    return result;
}