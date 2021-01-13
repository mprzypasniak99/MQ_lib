#include "MQ_lib.hpp"

int main() {
    ServerConnection sc;
    std::string l = "user";
    std::string p = "login";

    if(sc.requestRegistration(l, p)) {
        printf("Successfuly registered!\n");
    }
    else {
        printf("Failed to register\n");
    }

    if(sc.logIn(l, p)) {
        printf("Successfuly logged in!\n");
    }
    else {
        printf("Failed to log in\n");
    }

    if(sc.createQueue("New Queue", false)) {
        printf("Successfuly created queue\n");
    }
    else {
        printf("Failed to create queue\n");
    }

    sc.requestQueueList();

    sc.joinQueue("New Queue");

    sc.addMessage("New Queue", "Test message", 60000);

    sc.logOut();

    sc.disconnect();
}