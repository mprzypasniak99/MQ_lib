/*
Uses timercpp library: https://github.com/99x/timercpp

License

MIT License

Copyright (c) 2018 Shalitha Suranga

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include "./timercpp/timercpp.h"
#include "AbstractQueue.hpp"
#include "AbstractMessage.hpp"


class Message: public AbstractMessage
{
private:
    std::string sender; // name of the sending user
    
    Timer timer; // timer to check if message is not expired
    
    std::string contents; // contents of the message

    AbstractQueue *queue; // queue that message is stored in
public:
    Message(const char* sender, long time, const char* message, AbstractQueue *q);

    ~Message();

    void handleTimer();

    /* =========== SETTERS ============= */
    void setNextMessage(Message* m);

    void setPrevMessage(Message* m);

    /* =========== GETTERS ============= */
    std::string getSender();

    std::string getContents();
    
    // pointers for bidirectional list
    Message* previousMessage;

    Message* nextMessage;
};


#endif