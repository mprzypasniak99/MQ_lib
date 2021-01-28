#include "timercpp.h"


void Timer::setTimeout(std::function<void()> function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        using namespace std::chrono_literals;

        if(this->clear) return;
        std::this_thread::sleep_for(delay * 1s);
        if(this->clear) return;
        function();
    });
    t.detach();
}

void Timer::setInterval(std::function<void()> function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(this->clear) return;
            function();
        }
    });
    t.detach();
}

void Timer::stop() {
    this->clear = true;
}