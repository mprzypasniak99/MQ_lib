#ifndef CONNECTIONMONITOR_HPP
#define CONNECTIONMONITOR_HPP

#include <condition_variable>
#include <mutex>
#include <chrono>

class ConnectionMonitor
{
private:
    std::mutex mtx;
    std::chrono::time_point<std::chrono::system_clock> lastRead;
    std::chrono::time_point<std::chrono::system_clock> lastWrite;
    bool isTimedOut;
public:
    std::condition_variable condVar;
    ConnectionMonitor();
    
    void updateRead();
    void updateWrite();
    
    void confirmMessage();

    bool timeout(bool& isFirst);

    bool getIsTimedOut();
};

#endif