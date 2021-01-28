#include "../../lib/MQ_lib.hpp"
#include "../../lib/MessageHandler.hpp"
#include <vector>

void response(uint16_t code) {
    if(code >= 200 && code < 300) {
        printf("Success!\n");
    }
    else {
        printf("Failure!\n");
    }
    
}

void messageResponse(std::string q, std::string user, std::string message) {
    printf("New message in queue %s from user %s:\n%s\n", q.c_str(), user.c_str(), message.c_str());
}

void queueListResponse(uint16_t code, std::vector<std::string> list) {
    if(code >= 200 && code < 300) {
        printf("Successfully received queue list!\nList:\n");
        for(unsigned int i = 0; i < list.size(); i++) {
            printf("-%s\n", list[i].c_str());
        }
    }
    else {
        printf("Failure!\n");
    }
}

int main(int argc, char** argv) {
    ServerConnection sc("../../config/connection.json");
    std::string l = "user";
    std::string p = "login";

    std::string l2 = "admin";
    std::string p2 = "123";

    MessageHandler handler(&sc);

    handler.setAddMessageResponse(response);
    handler.setCreateQueueResponse(response);
    handler.setDeleteQueueResponse(response);
    handler.setDeleteUserResponse(response);
    handler.setJoinQueueResponse(response);
    handler.setLeaveQueueResponse(response);
    handler.setLogInResponse(response);
    handler.setLogOutResponse(response);
    handler.setMessageResponse(messageResponse);
    handler.setRegistrationResponse(response);
    handler.setQueueListRespone(queueListResponse);

    handler.loop();

    sc.logIn(l, p);
    sc.createQueue("New Queue", false);
    sc.logOut();

    sc.logIn(l2,p2);
    sc.joinQueue("New Queue");
    sc.addMessage("New Queue", "Test message", 10);
    sc.logOut();
    
    sleep(10);
    
    sc.logIn(l,p);
    sleep(5);
    sc.logOut();
    sc.disconnect();
}