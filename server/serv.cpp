/*
Project uses rapidjson library for parsing json files
Github: https://github.com/Tencent/rapidjson
*/
#include "../rapidjson/document.h"
#include "../rapidjson/filewritestream.h"
#include "../rapidjson/prettywriter.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <signal.h>
#include "./Queue.hpp"
#include <map>
#include <vector>
#include <string>

// Abstract class used for handling different kinds of events
class Handler {
    public:
        virtual void handleEvent(uint32_t events) = 0;
        virtual ~Handler() = default;
};

// following two declarations are here just to allow declaration of Server class

class Client; 

class ServerHandler;

class Server {
    private:
        std::unordered_set<Client*> active_users; // holds users that are currently connected to the server
        int in_socket; // socket on which program listens to incoming connections
        int max_length; // max message length
        int def_time; // default message validity time (in seconds)
        int port; // port under which server will listen to new connections
        int epoll_fd; // file descriptor for epoll
        ServerHandler* handler; // handler for new connections

        std::map<std::string, Queue*> queues; // all queues

        void setReuseAddr(int sock);
    public:
        Server();
        rapidjson::Document readJsonFile(const char* filepath); // used to read and parse json config files

        void writeJsonFile(rapidjson::Document *d, const char *filepath); // used to write modified Json file to file

        // getters for handlers to use when they need access to certain fields
        int getEpoll();
        int getSocket();
        std::unordered_set<Client*>* getClients();

        std::vector<std::string> getAllQueues(Client* requester); // done

        bool createQueue(Client* owner, bool is_private, std::string name); // done

        bool deleteQueue(std::string name, Client* requester); // done

        bool joinQueue(Client* requester, std::string name);

        bool inviteToQueue(Client* requester, std::string qName, std::string username);

        bool leaveQueue(Client* requester, std::string name);

        bool getAllMessages(Client* requester, Queue *q); // send all unread messages from given queue to user

        void sendMessageToAll(Queue* queue); // send unread messages to active users in given queue

        bool addMessage(Client* requester, std::string queue, std::string content, long validityTime); // let's say that this is done

        bool authentication(std::string input, std::string *name); 
        // authentication - checks whether received username and password match data in user list

        bool registerUser(std::string name); // register new user in the server

        bool deleteUser(std::string name); // delete user

        void run(); // main loop of the server
};

// Client class stores connected user's data and handles events on his socket

class Client : public Handler { 
    private:
        int clientSocket; // socket through which communication with user is being done
        std::string username; // username taken from file - only if client is logged in
        Server* serv; // pointer to server, used to access data about other users and MQs

        // this function handles log in request
        void logIn(uint16_t *confirm) {
            std::string name = ""; // will be used to save username after authentication
            
            std::string message = ""; // will store login and password received from client

            // Read credentials sent by client and verify if they are correct
            readToBuffer(&message);
            
            if(serv->authentication(message, &name)) {
                username = name;
                printf("Successful login of user %s\n", username.c_str());
            
                *confirm = 200; // code to be sent to client, confirming success in logging in
            }
            else{
                printf("Failed to log in\n");
                *confirm = 401; // failure code
            }
        }

        void logOut(uint16_t *confirm) {
            if(username.compare("") == 0) {
                *confirm = 400;
            }
            else {
                username = "";
                *confirm = 200;
            }
        }

        void registerUser(uint16_t *confirm) {
            std::string message = "";

            readToBuffer(&message);

            if(serv->registerUser(message)) {
                printf("Successful registration with credentials %s\n", message.c_str());

                *confirm = 201; // code to be sent to client, confirm success in logging in
            }
            else
            {
                printf("Failed to register %s\n", message.c_str());

                *confirm = 409; // failure code   
            }
        }

        void deleteUser(uint16_t *confirm) {
            if(this->username.compare("") != 0 && serv->deleteUser(username)) { // check if user is logged in
                
                *confirm = 200;
            }
            else {
                *confirm = 401;
            }
        }

        void sendQueueList(uint16_t *confirm) {
            auto list = serv->getAllQueues(this);
            
            std::string message = "";
            *confirm = 400;
            
            if(list.size() > 0) {
                message = list[0];
                
                for(unsigned int i = 1; i < list.size(); i++) {
                    message += "," + list[i];
                }

                *confirm = 200;
            }

            uint16_t size = message.size();

            write(clientSocket, &size, sizeof(uint16_t));
            write(clientSocket, message.c_str(), size);
        }

        void joinQueue(uint16_t *confirm) {
            std::string buf;

            readToBuffer(&buf);
            
            if(serv->joinQueue(this, buf)) *confirm = 200;
            else *confirm = 400;
        }

        void leaveQueue(uint16_t *confirm) {
            std::string buf;

            readToBuffer(&buf);
            
            if(serv->leaveQueue(this, buf)) *confirm = 200;
            else *confirm = 400;
        }

        void createQueue(uint16_t *confirm) {
            std::string buf;

            readToBuffer(&buf);

            bool isPrivate;

            read(clientSocket, &isPrivate, sizeof(bool));
            
            if(serv->createQueue(this, isPrivate, buf)) *confirm = 200;
            else *confirm = 400;
        }

        void deleteQueue(uint16_t *confirm) {
            std::string buf;

            readToBuffer(&buf);
            
            if(serv->deleteQueue(buf, this)) *confirm = 200;
            else *confirm = 400;
        }

        void addMessage(uint16_t *confirm) {
            std::string name;

            readToBuffer(&name);

            std::string content;

            readToBuffer(&content);
            
            uint16_t validityTime;

            read(clientSocket, &validityTime, sizeof(uint16_t));

            if(serv->addMessage(this, name, content, validityTime)) *confirm = 200;
            else *confirm = 400;
        }

        void inviteUser(uint16_t *confirm) {
            std::string queue;

            readToBuffer(&queue);

            std::string user;

            readToBuffer(&user);

            if(serv->inviteToQueue(this, queue, user)) *confirm = 200;
            else *confirm = 400;
        }

        void readToBuffer(std::string *message) {
            uint16_t size = 0; // how many bates of data are to be read
    
            read(clientSocket, &size, sizeof(uint16_t)); // read size of message to be received

            char *buf = new char[size+1]; // prepare buffer for message
            buf[size] = '\0';

            int ammountRead = 0; // how many bytes of data have already been read
            
            // read until whole message is read
            while(ammountRead < size) {
                ammountRead += read(clientSocket, buf + ammountRead, size - ammountRead);
            }
            *message = buf;
        }

    public:
        // main function used for handling events
        virtual void handleEvent(uint32_t events) override {
            if(events & EPOLLIN) { // checks whether event that occured is about possible input
                
                uint16_t choice; // variable storing data read from socket - type of request
                
                // check if read is really possible
                if(read(clientSocket, &choice, sizeof(uint16_t)) > 0) {
                    
                    uint16_t confirm; // variable for storing request status
                    
                    switch (choice) // execute proper methods according to received request type
                    {
                    case 1: // LOG IN
                        logIn(&confirm);
                        break;
                    case 2: // LOG OUT
                        logOut(&confirm);
                        break;
                    case 3: // REGISTER NEW USER
                        registerUser(&confirm);
                        break;
                    case 4: // DELETE USER
                        deleteUser(&confirm);
                        username = "";
                        break;
                    case 5: // DISCONNECT
                        serv->getClients()->erase(this);
                        delete this;
                        return;
                        break;
                    case 6: // SEND ALL QUEUES
                        sendQueueList(&confirm);
                        break;
                    case 7: // JOIN QUEUE
                        joinQueue(&confirm);
                        break;
                    case 8: // LEAVE QUEUE
                        leaveQueue(&confirm);
                        break;
                    case 9: // CREATE QUEUE
                        createQueue(&confirm);
                        break;
                    case 10: // DELETE QUEUE
                        deleteQueue(&confirm);
                        break;
                    case 11: // SEND MESSAGE TO QUEUE
                        addMessage(&confirm);
                        break;
                    case 12: // INVITE USER TO QUEUE
                        inviteUser(&confirm);
                        break;
                    default: // different - wrong request or network error
                        confirm = 404;
                        break;
                    }

                    write(clientSocket, &confirm, sizeof(uint16_t)); // send status code to client
                }
                
            }
        }
        std::string getUsername(){
            return username;
        }

        int getSocket() {
            return clientSocket;
        }

        /* =============== CONSTRUCTOR ================*/
        Client(int fd, Server* serv){
            // save all necesarry values to object fields
            this->clientSocket = fd;
            this->serv = serv;
            this->username = "";

            epoll_event ee = {EPOLLIN|EPOLLRDHUP, {.ptr = this}}; // prepare event for epoll
            epoll_ctl(serv->getEpoll(), EPOLL_CTL_ADD, fd, &ee); // add that event to epoll
        }

        /* ============== DESTRUCTOR ================*/
        virtual ~Client(){
            epoll_ctl(serv->getEpoll(), EPOLL_CTL_DEL, clientSocket, nullptr); // delete event from epoll
            shutdown(clientSocket, SHUT_RDWR); // shut down the connection
            close(clientSocket); // close file descriptor
        }
};


// class for handling Server socket events - accepting new connections
class ServerHandler : public Handler {
    private:
        Server* serv; // serer pointer - used to add users to active clients set
    public:
        // main event handling function
        virtual void handleEvent(uint32_t events) override{
            if(events & EPOLLIN) { // check whether event is about new input
                
                sockaddr_in clientAddr{}; // struct for logging client's IP and port

                socklen_t len = sizeof(clientAddr); // length of structure
                
                int new_conn = accept(serv->getSocket(), (sockaddr*)&clientAddr, &len);
                // accept new connection and handle possible failure    
                    if(new_conn == -1) {
                        error(1, errno, "accept failed");
                    }
                    
                    // log connected user's IP and port
                    printf("New connection from %s:%d accepted\n", 
                    inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    
                    // create new Client object and insert it to connected clients set
                    serv->getClients()->insert(new Client(new_conn, serv));
            }
            if(events & ~EPOLLIN){ // other events
                error(0, errno, "Event %x on server socket", events);
            }
        }

        ServerHandler(Server* serv){
            this->serv = serv;
        }

        ~ServerHandler(){
            serv = nullptr;
        }
};



// allow server to use the same port after exit, without waiting
void Server::setReuseAddr(int sock)
{
    const int one = 1;
    int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(res) error(1,errno, "setsockopt failed");
}


/* ================ CONSTRUCTOR ===============*/
Server::Server() {
    rapidjson::Document d = readJsonFile("./config/config.json"); // read and parse config file

    // get necessary data from config file 
    max_length = d["max_length"].GetInt();
    def_time = d["def_time"].GetInt();
    port = d["port"].GetInt();

    // create socket and handle errors
    in_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(in_socket == -1) error(1, errno, "socket failed");
    setReuseAddr(in_socket);

    // configure socket parameters and bind them
    sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
    
    int res = bind(in_socket, (sockaddr*) &serverAddr, sizeof(serverAddr));
    
    // handle errors
    if(res) error(1, errno, "bind failed");

    // start listening on created socket
    res = listen(in_socket, 1);
    if(res) error(1, errno, "listen failed");

    // create handler for new connections
    handler = new ServerHandler(this);
    
    // create epoll instance
    epoll_fd = epoll_create1(0);

    // create epoll event for handling new connections
    epoll_event ee = {EPOLLIN, {.ptr = handler}};

    // add that event to epoll instance
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, in_socket, &ee);
}

// Read and parse json file
rapidjson::Document Server::readJsonFile(const char* filepath) {
    int fd = open(filepath, O_RDONLY); // open chosen file and handle errors
    if(fd == -1) {
        error(1, errno, "Failed to read file %s", filepath);
    }

    // create buffer for file
    std::string file = "";
    char buf[256] = { 0 };

    // read file
    while(read(fd, buf, 256) > 0) {
        file += buf;
    }
    close(fd);

    rapidjson::Document d;
    d.Parse(file.c_str()); // parse read file and return effects

    return d;
}

void Server::writeJsonFile(rapidjson::Document *d, const char *filepath) {
    // open file and delete it's contents
    FILE *fp = fopen(filepath, "w+");

    // prepare rapidjson wrapper for writing json to file
    char writeBuffer[4096];
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

    // create Writer object, managing writing json to file
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);

    d->Accept(writer);

    fclose(fp);
}

// authenticate user - read data from socket and compare with data in users list file
bool Server::authentication(std::string input, std::string *name) {

    int coma = input.find(","); // find coma - separator of login and password
    
    std::string login = input.substr(0, coma); // separate login from string
    std::string pass = input.substr(coma+1); // separate password from string

    rapidjson::Document d = readJsonFile("config/users.json"); // read file with users

    // check if login is on user list
    if(d.FindMember(login.c_str()) == d.MemberEnd()) {
        return false;
    }
    else { // check if password matches with user lsit
        if(pass.compare(d[login.c_str()].GetString()) != 0) {
            return false;
        }
    }
    *name = login; // save login so it can be accessed outside this method
    return true;
}

bool Server::registerUser(std::string name) {

    int coma = name.find(","); // find coma - separation between login and password
    
    std::string login = name.substr(0, coma);
    std::string pass = name.substr(coma + 1);

    rapidjson::Document d = readJsonFile("config/users.json"); // read and parse current list of users
    
    // check whether new user isn't already registered
    if(d.FindMember(login.c_str()) == d.MemberEnd()) { 
        
        auto& allocator = d.GetAllocator();

        // add user credentials to json document structure
        d.AddMember(rapidjson::Value(login.c_str(), allocator),
                    rapidjson::Value(pass.c_str(), allocator),
                    allocator);

        writeJsonFile(&d, "config/users.json"); // save modifications to the same file

        return true; 
    }
    else { // if user is already registered, inform user about failure
        return false;
    }
}

bool Server::deleteUser(std::string name) {

    rapidjson::Document d = readJsonFile("config/users.json"); // read file with users

    // check if login is on user list
    if(d.FindMember(name.c_str()) == d.MemberEnd()) {
        return false;
    }

    if(!d.EraseMember(name.c_str())) {
        return false;
    }

    writeJsonFile(&d, "config/users.json");

    return true;
}

/* ============ GETTERS ============== */
int Server::getEpoll() {
    return epoll_fd;
}

int Server::getSocket() {
    return in_socket;
}

std::unordered_set<Client*>* Server::getClients() {
    return &active_users;
}

/* ============= MAIN LOOP =============== */
void Server::run() {
    epoll_event ee = {}; // structure for event that currently occurs
    while(true){
        if(-1 == epoll_wait(epoll_fd, &ee, 1, -1)) { // wait for events
            error(0,errno,"epoll_wait failed");
        }
        ((Handler*)ee.data.ptr)->handleEvent(ee.events); // use apropriate method to handle event
    }

}


// had to comment out this part during tests, because of some problems with compilation in VS
// leaving it for now
// you should comment VS

std::vector<std::string> Server::getAllQueues(Client* requester){
    std::vector<std::string> ret;
    for(const auto& [name, queue] : queues){    
        if(!queue->getPrivacy() || requester->getUsername() == queue->getOwner()){ // if queue is private and requester is not owner, then he can't see the queue
            ret.push_back(name);
        }
    }
    return ret;
}

bool Server::createQueue(Client* owner, bool is_private, std::string name){
    std::string username = owner->getUsername();
    auto it = queues.find(name); // take iterator
    if (it != queues.end()) return false; //checks if queue with given name already exists
    Queue *newQueue = new Queue(username.c_str(), is_private, name.c_str()); // creates queue
    queues.insert(std::make_pair(name, newQueue)); //inserts queue on list
    return true;
}

bool Server::deleteQueue(std::string name, Client* requester){
    auto it = queues.find(name); // take iterator
    if (it != queues.end()){
        if(it->second->getOwner() != requester->getUsername()) return false; //check if requester is owner
        queues.erase (it); //delete queue if exists
        return true;
    }
    return false;
}

bool Server::joinQueue(Client* requester, std::string name){
    auto it = queues.find(name);
    if (it != queues.end()){
        return it->second->addQueueClient(requester->getUsername());
    }
    return false;
}

bool Server::leaveQueue(Client* requester, std::string name) {
    auto it = queues.find(name);
    if (it != queues.end()) {
        return it->second->deleteQueueClient(requester->getUsername());
    }
    return false;
}

bool Server::inviteToQueue(Client *requester, std::string qName, std::string username) {
    rapidjson::Document d = readJsonFile("config/users.json");

    if(d.FindMember(username.c_str()) == d.MemberEnd()) {
        return false;
    }

    auto it = queues.find(qName);
    if(it != queues.end()) {
        if(it->second->getOwner().compare(requester->getUsername()) != 0) {
            return false;
        }

        return it->second->addQueueClient(username);
    }
    return false;
}

bool Server::addMessage(Client* requester, std::string queue, std::string content, long validityTime){
    auto it = queues.find(queue);
    if (it != queues.end()){
            it->second->addMessage(new Message(requester->getUsername().c_str(), validityTime, content.c_str(), it->second));
            
            sendMessageToAll(queues[queue]);
            return true;
    }
    return false;
}

bool Server::getAllMessages(Client* requester, Queue *q) {
    MessageMonitor *userMonitor = q->getUserMonitor(requester->getUsername());

    if(userMonitor == nullptr) {
        return false;
    }
    else {
        Message * lastMessage = userMonitor->getMessage();
            
        int clientSocket = requester->getSocket();

        while(lastMessage != q->getLastMessage() && lastMessage != nullptr) {
            std::string message = q->getName() + "," + lastMessage->getSender() + "," +
            lastMessage->getContents();
                
            uint16_t type = 21;
            uint16_t size = message.size();
                
            write(clientSocket, &type, sizeof(uint16_t));
            write(clientSocket, &size, sizeof(uint16_t));
            write(clientSocket, message.c_str(), size);

            userMonitor->nextMessage();
            lastMessage = userMonitor->getMessage();
        }

        return true;
    }
}

void Server::sendMessageToAll(Queue *queue) {
    for(Client *client : active_users) {
        if(client->getUsername().compare("") == 0) {
            continue;
        }

        getAllMessages(client, queue);
    }
}

int main() {
    Server serv;
    serv.run();
}