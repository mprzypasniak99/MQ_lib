#include "ConnectionMonitor.hpp"

ConnectionMonitor::ConnectionMonitor() {
    updateRead();
    updateWrite();
    isTimedOut = false;
}

void ConnectionMonitor::updateRead() {
    std::unique_lock<std::mutex> lock(mtx);
    lastRead = std::chrono::system_clock::now();
}

void ConnectionMonitor::updateWrite() {
    std::unique_lock<std::mutex> lock(mtx);
    lastWrite = std::chrono::system_clock::now();
}

void ConnectionMonitor::confirmMessage() {
    std::unique_lock<std::mutex> lock(mtx);
    condVar.notify_all();
}

bool ConnectionMonitor::timeout(bool& isFirst) {
    using namespace std::chrono_literals;

    int timeOutVal = 2000; // timeout - wait for response for 2000ms
    if(!isTimedOut){
        std::unique_lock<std::mutex> lock(mtx);
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        condVar.wait_for(lock, timeOutVal * 1ms);
        // thread waits at this point either for time equal to timeOutVal or less if other thread
        // receives response from the client
        std::chrono::time_point<std::chrono::system_clock> stop = std::chrono::system_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        if(duration.count() > timeOutVal) { // check if connection isn't already timed out
            auto timeSinceWrite = std::chrono::duration_cast<std::chrono::milliseconds>(stop - lastWrite);
            auto timeSinceRead = std::chrono::duration_cast<std::chrono::milliseconds>(stop - lastRead);
            
            /*  check how much time has passed since last successful
                read/write */
            if( timeSinceRead.count() > 3000
                && timeSinceWrite.count() > 3000) {
                    isTimedOut = true;
                    isFirst = true; 
                    /*  the first thread which discovers that connection is timed out has to initiate
                        shutdown of this connection */ 
            }
        }
    }

    return isTimedOut;
}

bool ConnectionMonitor::getIsTimedOut() {
    return isTimedOut;
}