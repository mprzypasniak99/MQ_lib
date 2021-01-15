#ifndef RESOURCEMONITOR_HPP
#define RESOURCEMONITOR_HPP

#include <mutex>

class ResourceMonitor
{
private:
    std::mutex wmutex; // mutex for enabling only one thread to edit data in queue
    std::mutex tryEnter;
    std::mutex rmutex;
    std::mutex resource;

    int read;
    int write;
public:
    void enterRead();
    void exitRead();
    void enterWrite();
    void exitWrite();

    ResourceMonitor();
};

#endif