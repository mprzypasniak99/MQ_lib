/*
Project uses rapidjson library for parsing json files
Github: https://github.com/Tencent/rapidjson
*/
#include "../rapidjson/document.h"
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

        void setReuseAddr(int sock);
    public:
        Server();
        rapidjson::Document readJsonFile(const char* filepath); // used to read and parse json config files

        // getters for handlers to use when they need access to certain fields
        int getEpoll();
        int getSocket();
        std::unordered_set<Client*>* getClients();

        bool authentication(int conn, std::string *name); 
        // authentication - checks whether received username and password match data in user list
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

            if(serv->authentication(clientSocket, &name)) {
                username = name;
                printf("Successful login of user %s\n", username.c_str());
            
                *confirm = htons(200); // code to be sent to client, confirming success in logging in
            }
            else{
                printf("Failed to log in\n");
                *confirm = htons(401); // failure code
            }
        }

    public:
        // main function used for handling events
        virtual void handleEvent(uint32_t events) override {
            if(events & EPOLLIN) { // checks whether event that occured is about possible input
                
                uint16_t choice; // variable storing data read from socket - type of request
                
                // check if read is really possible
                if(read(clientSocket, &choice, sizeof(uint16_t)) > 0) {
                    
                    choice = ntohs(choice); // convert read value
                    
                    uint16_t confirm; // variable for storing request status
                    
                    switch (choice) // execute proper methods according to received request type
                    {
                    case 1: // LOG IN
                        logIn(&confirm);
                        break;
                    case 2: // LOG OUT
                        serv->getClients()->erase(this);
                        delete this;
                        return;
                        break;
                    default: // different - wrong request or network error
                        confirm = htons(404);
                        break;
                    }

                    write(clientSocket, &confirm, sizeof(uint16_t)); // send status code to client
                }
                
            }
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

// authenticate user - read data from socket and compare with data in users list file
bool Server::authentication(int conn, std::string *name) {
    std::string input = ""; // string to make operation on read data easier

    uint16_t size = 0; // how many bates of data are to be read
    
    read(conn, &size, sizeof(uint16_t)); // read size of message to be received
    
    size = ntohs(size); // convert read value

    char buf[size]; // prepare buffer for login and password
    
    int ammountRead = 0; // how many bytes of data have already been read
    
    // read until whole message is read
    while(ammountRead < size) {
        ammountRead += read(conn, buf + ammountRead, size - ammountRead);
    } 
    
    input = buf; // put message into string

    int coma = input.find(","); // find coma - separator of login and password
    int semic = input.find(";"); // find semicolon - end of message - POSSIBLY CAN BE CHANGED
    std::string login = input.substr(0, coma); // separate login from string
    std::string pass = input.substr(coma+1, semic - coma - 1); // separate password from string

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

int main() {
    Server serv;
    serv.run();
}