#include "login.h"
#include "ui_login.h"
#include <functional>



Login::Login(QWidget *parent, ServerConnection* conn, MessageHandler* handle, Widget* main) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    connect(ui->loginButton, &QPushButton::clicked, this, &Login::logIn);
    connect(ui->registerButton, &QPushButton::clicked, this, &Login::registerUser);

    handler = handle;

    handler->setLogInResponse(std::function<void(uint16_t)>([&](uint16_t status){
                                  logInResponse(status);
                              }));
    handler->setRegistrationResponse(std::function<void(uint16_t)>([&](uint16_t status){
                                         registerUserResponse(status);
                                     }));

    connection = conn;
    mainScreen = main;
}

Login::~Login()
{
    delete ui;
}

void Login::logIn() {
    QString login = ui->loginField->text();
    QString password = ui->passwordField->text();

    connection->logIn(login.toStdString(), password.toStdString());
}

void Login::registerUser() {
    QString login = ui->loginField->text();
    QString password = ui->passwordField->text();

    connection->requestRegistration(login.toStdString(), password.toStdString());
}

void Login::logInResponse(uint16_t status) {
    ui->loginField->setText("");
    ui->passwordField->setText("");

    if(status >=200 && status < 300) {
        ui->statusMessage->setText("Successfuly logged in");
        mainScreen->show();
        mainScreen->setUp();
        hide();
    }
    else {
        ui->statusMessage->setText("Failed to log in");
    }
}

void Login::registerUserResponse(uint16_t status) {
    if(status >=200 && status < 300) {
        ui->statusMessage->setText("Successfuly registered");
    }
    else {
        ui->statusMessage->setText("Failed to register");
    }
}
