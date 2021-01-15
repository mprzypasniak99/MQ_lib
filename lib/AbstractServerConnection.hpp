#ifndef ABSTRACTSERVER_HPP
#define ABSTRACTSERVER_HPP

#include <cstdint>
#include <mutex>

class AbstractServerConnection {
    protected:
        int serverSocket;

        uint16_t maxLength; // max length of message
        
        uint16_t defaultTime; // default validity time

    public:
        virtual void setMaxLength(uint16_t maxL) = 0;
        virtual void setDefaultValidity(uint16_t defT) = 0;

        virtual int getSocket() = 0;

        std::mutex responseMutex; // mutex preventing sending new requests before receiving response
};

#endif