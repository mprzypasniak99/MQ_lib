#include "widget.h"
#include "ui_widget.h"
#include <functional>
#include <thread>


Widget::Widget(QWidget *parent, ServerConnection* conn, MessageHandler* handle) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connection = conn;

    handler = handle;

    connect(ui->loginButton, &QPushButton::clicked, this, &Widget::logIn);
    connect(ui->registerButton, &QPushButton::clicked, this, &Widget::registerUser);
    connect(ui->newQueueName, &QLineEdit::textChanged, this, &Widget::controlNewQueueName);
    connect(ui->messageField, &QTextEdit::textChanged, this, &Widget::controlMessage);
    connect(ui->createQueue, &QPushButton::clicked, this, &Widget::createQueue);
    connect(ui->sendMessage, &QPushButton::clicked, this, &Widget::sendMessage);
    connect(ui->queues, &QListWidget::doubleClicked, this, &Widget::joinQueue);
    connect(ui->queues, &QListWidget::currentRowChanged, this, &Widget::controlSelectedQueue);
    connect(ui->leaveQueueButton, &QPushButton::clicked, this, &Widget::leaveQueue);
    connect(ui->refreshButton, &QPushButton::clicked, this, &Widget::refreshQueueList);
    connect(ui->deleteQueueButton, &QPushButton::clicked, this, &Widget::deleteQueue);
    connect(ui->addUserButton, &QPushButton::clicked, this, &Widget::inviteUser);
    connect(ui->logoutButton, &QPushButton::clicked, this, &Widget::logOut);

    handler->setQueueListRespone(std::function<void(uint16_t, std::vector<std::string>)>(
                                     [&](uint16_t status, std::vector<std::string> list){
        queueListResponse(status, list);
    }));
    handler->setLogInResponse(std::function<void(uint16_t)>([&](uint16_t status){
                                  logInResponse(status);
                              }));
    handler->setRegistrationResponse(std::function<void(uint16_t)>([&](uint16_t status){
                                         registerUserResponse(status);
                                     }));
    handler->setCreateQueueResponse([&](uint16_t status){
        createQueueResponse(status);
    });

    handler->setAddMessageResponse(std::function<void(uint16_t)>([&](uint16_t status){
        sendMessageResponse(status);
    }));

    handler->setMessageResponse([&](std::string queue, std::string user, std::string content){
        receiveMessageResponse(queue, user, content);
    });

    handler->setJoinQueueResponse([&](uint16_t status){
        joinQueueResponse(status);
    });

    handler->setLeaveQueueResponse([&](uint16_t status){
        leaveQueueResponse(status);
    });

    handler->setUserListResponse([&](uint16_t status, std::vector<std::string> list){
        userListResponse(status, list);
    });

    handler->setDeleteQueueResponse([&](uint16_t status){
        deleteQueueResponse(status);
    });

    handler->setInviteUserResponse([&](uint16_t status){
        inviteUserResponse(status);
    });

    handler->setLogOutResponse([&](uint16_t status){
        logOutResponse(status);
    });


}

Widget::~Widget()
{
    delete ui;
}


void Widget::queueListResponse(uint16_t status, std::vector<std::string> list) {
    if( status < 300 && status >= 200) {
        ui->queues->clear();
        for(unsigned int i = 0; i < list.size(); i++) {
            ui->queues->addItem(new QListWidgetItem(QString(list[i].c_str())));
        }
    }
    else {
        ui->queues->clear();
        ui->queues->addItem("No queues");
    }


}

void Widget::receiveMessageResponse(std::string queue, std::string user, std::string content) {
    std::string message = user + " sent message to " + queue + ": " + content;

    ui->messagesList->addItem(message.c_str());
}

void Widget::logIn() {
    QString login = ui->username->text();
    QString password = ui->password->text();

    connection->logIn(login.toStdString(), password.toStdString());
}

void Widget::registerUser() {
    QString login = ui->username->text();
    QString password = ui->password->text();

    connection->requestRegistration(login.toStdString(), password.toStdString());
}

void Widget::logInResponse(uint16_t status) {
    QString statusMessage = "You are logged in as " + ui->username->text();
    ui->username->setText("");
    ui->password->setText("");

    if(status >=200 && status < 300) {
        ui->statusMessage->setText("Successfuly logged in");
        ui->loginStatus->setText(statusMessage);
        ui->messagesList->clear();

        ui->messageField->setEnabled(true);
        ui->messagesList->setEnabled(true);
        ui->queues->setEnabled(true);
        ui->users->setEnabled(true);
        ui->newQueueName->setEnabled(true);
        ui->validityTime->setEnabled(true);
        ui->refreshButton->setEnabled(true);
        ui->logoutButton->setEnabled(true);
        ui->inviteUsername->setEnabled(true);
        ui->addUserButton->setEnabled(true);
        ui->validityTime->setValue(connection->getDefaultValidity());

        ui->addUserButton->setEnabled(false);
        ui->loginButton->setEnabled(false);
        ui->password->setEnabled(false);
        ui->registerButton->setEnabled(false);
        ui->username->setEnabled(false);

        std::thread t([&](){
            connection->requestQueueList();
        });
        t.detach();
    }
    else {
        ui->statusMessage->setText("Failed to log in");
    }
}

void Widget::registerUserResponse(uint16_t status) {
    if(status >=200 && status < 300) {
        ui->statusMessage->setText("Successfuly registered");
    }
    else {
        ui->statusMessage->setText("Failed to register");
    }
}

void Widget::controlNewQueueName() {
    if(ui->newQueueName->text().length() == 0) {
        ui->createQueue->setEnabled(false);
        ui->privateCheckbox->setEnabled(false);
    }
    else {
        ui->createQueue->setEnabled(true);
        ui->privateCheckbox->setEnabled(true);
    }
}

void Widget::controlMessage() {
    QString content = ui->messageField->toPlainText();
    if(content.length() == 0 || ui->queues->currentRow() == -1) {
        ui->sendMessage->setEnabled(false);
    }
    else {
        ui->sendMessage->setEnabled(true);
    }

    if(content.length() > connection->getMaxLength()) {
        content = content.left(content.length() - 1);
        ui->messageField->setText(content);
    }
}

void Widget::controlSelectedQueue() {
    if(ui->queues->currentRow() != -1) { // ensure that queue is selected
        ui->leaveQueueButton->setEnabled(true);
        ui->deleteQueueButton->setEnabled(true);
        ui->addUserButton->setEnabled(true);

        connection->requestUserListFromQueue(ui->queues->currentItem()->text().toStdString());
    }
    else {
        ui->leaveQueueButton->setEnabled(false);
        ui->deleteQueueButton->setEnabled(false);
        ui->users->clear();
    }
}

void Widget::createQueue() {
    std::string name = ui->newQueueName->text().toStdString();
    bool isPrivate = ui->privateCheckbox->isChecked();

    connection->createQueue(name, isPrivate);
}

void Widget::createQueueResponse(uint16_t status) {
    if(status >= 200 && status < 300) {
        QThread th();
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Successfuly created queue");
            ui->newQueueName->clear();

            connection->requestQueueList();
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
    else {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Failed to create queue");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
}

void Widget::sendMessage() {
    std::string content = ui->messageField->toPlainText().toStdString();
    std::string queue = ui->queues->currentItem()->text().toStdString();
    uint16_t validity = ui->validityTime->value();

    connection->addMessage(queue, content, validity);
}

void Widget::sendMessageResponse(uint16_t status) {
    if(status >= 200 && status < 300) {
        std::string content = ui->messageField->toPlainText().toStdString();
        std::string queue = ui->queues->currentItem()->text().toStdString();

        std::string message = "You sent message to ";
        message += queue + ": " + content;

        ui->messagesList->addItem(new QListWidgetItem(QString(message.c_str())));

        ui->messageField->setDocument(new QTextDocument(QString("")));

        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Successfuly sent message");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
    else {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Failed to send message");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
}

void Widget::joinQueue() {
    std::string queue = ui->queues->currentItem()->text().toStdString();

    connection->joinQueue(queue);
}

void Widget::joinQueueResponse(uint16_t status) {
    if(status >= 200 && status < 300) {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Successfuly joined queue");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
    else {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Failed to join queue");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
}

void Widget::leaveQueue() {
    std::string queue = ui->queues->currentItem()->text().toStdString();

    connection->leaveQueue(queue);
}

void Widget::leaveQueueResponse(uint16_t status) {
    if(status >= 200 && status < 300) {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Successfuly left queue");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
    else {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Failed to leave queue");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
}

void Widget::userListResponse(uint16_t status, std::vector<std::string> list) {
    ui->users->clear();
    if(status >= 200 && status < 300) {
        for(std::string i : list) {
            ui->users->addItem(new QListWidgetItem(QString(i.c_str())));
        }
    }
    else {
        ui->users->addItem(new QListWidgetItem("No users"));
    }
}

void Widget::refreshQueueList() {
    connection->requestQueueList();
}

void Widget::deleteQueue() {
    std::string queue = ui->queues->currentItem()->text().toStdString();

    connection->deleteQueue(queue);
}

void Widget::deleteQueueResponse(uint16_t status) {
    if(status >= 200 && status < 300) {
        std::thread t([&](){
            using namespace std::chrono_literals;
            connection->requestQueueList();
            ui->statusMessage->setText("Successfuly deleted queue");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
    else {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Failed to delete queue");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
}

void Widget::inviteUser() {
    std::string user = ui->inviteUsername->text().toStdString();
    std::string queue = ui->queues->currentItem()->text().toStdString();

    if(user.length() != 0) {
        connection->inviteUser(queue, user);
    }
    else {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Enter name of the user to invite");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
}

void Widget::inviteUserResponse(uint16_t status) {
    if(status >= 200 && status < 300) {
        std::thread t([&](){
            using namespace std::chrono_literals;
            ui->inviteUsername->clear();

            ui->statusMessage->setText("Successfuly invited user");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
    else {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Failed to invite user");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
}

void Widget::logOut() {
    connection->logOut();
}

void Widget::logOutResponse(uint16_t status) {
    if(status >= 200 && status < 300) {
        ui->messageField->setDocument(new QTextDocument(QString("")));
        ui->messagesList->clear();
        ui->queues->clear();
        ui->users->clear();
        ui->newQueueName->clear();
        ui->inviteUsername->clear();
        ui->loginStatus->setText("You are not logged in");

        ui->addUserButton->setEnabled(false);
        ui->createQueue->setEnabled(false);
        ui->deleteQueueButton->setEnabled(false);
        ui->inviteUsername->setEnabled(false);
        ui->leaveQueueButton->setEnabled(false);
        ui->logoutButton->setEnabled(false);
        ui->messageField->setEnabled(false);
        ui->messagesList->setEnabled(false);
        ui->newQueueName->setEnabled(false);
        ui->privateCheckbox->setEnabled(false);
        ui->queues->setEnabled(false);
        ui->refreshButton->setEnabled(false);
        ui->sendMessage->setEnabled(false);
        ui->users->setEnabled(false);
        ui->validityTime->setEnabled(false);

        ui->loginButton->setEnabled(true);
        ui->password->setEnabled(true);
        ui->registerButton->setEnabled(true);
        ui->username->setEnabled(true);

        std::thread t([&](){
            using namespace std::chrono_literals;
            ui->inviteUsername->clear();

            ui->statusMessage->setText("Successfuly logged out");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
    else {
        std::thread t([&](){
            using namespace std::chrono_literals;

            ui->statusMessage->setText("Failed to log out");
            std::this_thread::sleep_for(3s);
            ui->statusMessage->clear();
        });
        t.detach();
    }
}
