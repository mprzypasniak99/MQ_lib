#include "MQ_lib.hpp"

int main() {
    ServerConnection sc;
    std::string l = "user";
    std::string p = "login";
    if(sc.logIn(l.c_str(), p.c_str())) {
        printf("Successfuly logged in!\n");
    }
    else {
        printf("Failed to log in\n");
        return -1;
    }

    sc.logOut();
}