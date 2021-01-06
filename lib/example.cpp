#include "MQ_lib.hpp"

int main() {
    ServerConnection sc;
    std::string l = "panNowy";
    std::string p = "NicOTymNieWiem";

    if(sc.requestRegistration(l.c_str(), p.c_str())) {
        printf("Successfuly registered!\n");
    }
    else {
        printf("Failed to register\n");
    }

    

    if(sc.logIn(l.c_str(), p.c_str())) {
        printf("Successfuly logged in!\n");
    }
    else {
        printf("Failed to log in\n");
    }

    sc.logOut();
}