#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "../lib/MQ_lib.hpp"
#include "../lib/MessageHandler.hpp"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent, ServerConnection* conn, MessageHandler* handle);
    ~Widget();


private:
    Ui::Widget *ui;

    ServerConnection* connection;
    MessageHandler* handler;

    void queueListResponse(uint16_t status, std::vector<std::string> list);

    void receiveMessageResponse(std::string queue, std::string user, std::string content);

    void userListResponse(uint16_t status, std::vector<std::string> list);

    void logIn();
    void logInResponse(uint16_t status);

    void registerUser();
    void registerUserResponse(uint16_t status);

    void createQueue();
    void createQueueResponse(uint16_t status);

    void controlNewQueueName();
    void controlMessage();
    void controlSelectedQueue();

    void sendMessage();
    void sendMessageResponse(uint16_t status);

    void joinQueue();
    void joinQueueResponse(uint16_t status);

    void leaveQueue();
    void leaveQueueResponse(uint16_t status);

    void refreshQueueList();

    void deleteQueue();
    void deleteQueueResponse(uint16_t status);

    void inviteUser();
    void inviteUserResponse(uint16_t status);

    void logOut();
    void logOutResponse(uint16_t status);
};

#endif // WIDGET_H
