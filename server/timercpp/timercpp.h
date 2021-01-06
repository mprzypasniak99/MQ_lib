#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

class Timer {
    bool clear = false;

    public:
        void setTimeout(std::function<void()> function, int delay);
        void setInterval(std::function<void()> function, int interval);
        void stop();

};

#endif