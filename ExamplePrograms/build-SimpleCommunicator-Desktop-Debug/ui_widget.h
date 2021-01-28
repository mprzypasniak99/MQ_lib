/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QListWidget *messagesList;
    QListWidget *queues;
    QTextEdit *messageField;
    QPushButton *createQueue;
    QPushButton *sendMessage;
    QFrame *line;
    QLineEdit *username;
    QLineEdit *password;
    QLabel *label;
    QLabel *label_2;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *statusMessage;
    QLineEdit *newQueueName;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QCheckBox *privateCheckbox;
    QPushButton *addUserButton;
    QPushButton *refreshButton;
    QLineEdit *inviteUsername;
    QLabel *label_7;
    QSpinBox *validityTime;
    QLabel *label_8;
    QPushButton *leaveQueueButton;
    QListWidget *users;
    QPushButton *deleteQueueButton;
    QFrame *line_2;
    QLabel *loginStatus;
    QPushButton *logoutButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(639, 555);
        messagesList = new QListWidget(Widget);
        messagesList->setObjectName(QStringLiteral("messagesList"));
        messagesList->setEnabled(false);
        messagesList->setGeometry(QRect(150, 30, 331, 291));
        queues = new QListWidget(Widget);
        queues->setObjectName(QStringLiteral("queues"));
        queues->setEnabled(false);
        queues->setGeometry(QRect(10, 30, 121, 131));
        messageField = new QTextEdit(Widget);
        messageField->setObjectName(QStringLiteral("messageField"));
        messageField->setEnabled(false);
        messageField->setGeometry(QRect(150, 330, 331, 61));
        createQueue = new QPushButton(Widget);
        createQueue->setObjectName(QStringLiteral("createQueue"));
        createQueue->setEnabled(false);
        createQueue->setGeometry(QRect(10, 390, 121, 29));
        sendMessage = new QPushButton(Widget);
        sendMessage->setObjectName(QStringLiteral("sendMessage"));
        sendMessage->setEnabled(false);
        sendMessage->setGeometry(QRect(390, 400, 91, 29));
        line = new QFrame(Widget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 430, 621, 21));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        username = new QLineEdit(Widget);
        username->setObjectName(QStringLiteral("username"));
        username->setGeometry(QRect(80, 460, 171, 31));
        password = new QLineEdit(Widget);
        password->setObjectName(QStringLiteral("password"));
        password->setGeometry(QRect(80, 500, 171, 29));
        password->setEchoMode(QLineEdit::Password);
        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 460, 61, 21));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 500, 81, 21));
        loginButton = new QPushButton(Widget);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        loginButton->setGeometry(QRect(410, 460, 87, 29));
        registerButton = new QPushButton(Widget);
        registerButton->setObjectName(QStringLiteral("registerButton"));
        registerButton->setGeometry(QRect(410, 500, 87, 29));
        statusMessage = new QLabel(Widget);
        statusMessage->setObjectName(QStringLiteral("statusMessage"));
        statusMessage->setGeometry(QRect(270, 460, 121, 71));
        statusMessage->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        statusMessage->setWordWrap(true);
        newQueueName = new QLineEdit(Widget);
        newQueueName->setObjectName(QStringLiteral("newQueueName"));
        newQueueName->setEnabled(false);
        newQueueName->setGeometry(QRect(10, 330, 121, 29));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 10, 121, 17));
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(150, 10, 331, 20));
        label_4->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(500, 10, 131, 20));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setEnabled(true);
        label_6->setGeometry(QRect(10, 302, 121, 20));
        label_6->setAlignment(Qt::AlignCenter);
        privateCheckbox = new QCheckBox(Widget);
        privateCheckbox->setObjectName(QStringLiteral("privateCheckbox"));
        privateCheckbox->setEnabled(false);
        privateCheckbox->setGeometry(QRect(10, 360, 121, 22));
        addUserButton = new QPushButton(Widget);
        addUserButton->setObjectName(QStringLiteral("addUserButton"));
        addUserButton->setEnabled(false);
        addUserButton->setGeometry(QRect(500, 330, 131, 29));
        refreshButton = new QPushButton(Widget);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));
        refreshButton->setEnabled(false);
        refreshButton->setGeometry(QRect(10, 170, 121, 29));
        inviteUsername = new QLineEdit(Widget);
        inviteUsername->setObjectName(QStringLiteral("inviteUsername"));
        inviteUsername->setEnabled(false);
        inviteUsername->setGeometry(QRect(500, 290, 131, 29));
        label_7 = new QLabel(Widget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(503, 260, 121, 21));
        label_7->setAlignment(Qt::AlignCenter);
        validityTime = new QSpinBox(Widget);
        validityTime->setObjectName(QStringLiteral("validityTime"));
        validityTime->setEnabled(false);
        validityTime->setGeometry(QRect(260, 400, 101, 27));
        validityTime->setMaximum(3600);
        label_8 = new QLabel(Widget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(160, 400, 151, 21));
        leaveQueueButton = new QPushButton(Widget);
        leaveQueueButton->setObjectName(QStringLiteral("leaveQueueButton"));
        leaveQueueButton->setEnabled(false);
        leaveQueueButton->setGeometry(QRect(10, 220, 121, 29));
        users = new QListWidget(Widget);
        users->setObjectName(QStringLiteral("users"));
        users->setEnabled(false);
        users->setGeometry(QRect(500, 30, 131, 221));
        deleteQueueButton = new QPushButton(Widget);
        deleteQueueButton->setObjectName(QStringLiteral("deleteQueueButton"));
        deleteQueueButton->setEnabled(false);
        deleteQueueButton->setGeometry(QRect(10, 260, 121, 29));
        line_2 = new QFrame(Widget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(10, 200, 118, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        loginStatus = new QLabel(Widget);
        loginStatus->setObjectName(QStringLiteral("loginStatus"));
        loginStatus->setGeometry(QRect(520, 450, 101, 41));
        loginStatus->setWordWrap(true);
        logoutButton = new QPushButton(Widget);
        logoutButton->setObjectName(QStringLiteral("logoutButton"));
        logoutButton->setEnabled(false);
        logoutButton->setGeometry(QRect(520, 500, 87, 29));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        createQueue->setText(QApplication::translate("Widget", "Create queue", Q_NULLPTR));
        sendMessage->setText(QApplication::translate("Widget", "Send", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "Username:", Q_NULLPTR));
        label_2->setText(QApplication::translate("Widget", "Password:", Q_NULLPTR));
        loginButton->setText(QApplication::translate("Widget", "Login", Q_NULLPTR));
        registerButton->setText(QApplication::translate("Widget", "Register", Q_NULLPTR));
        statusMessage->setText(QString());
        label_3->setText(QApplication::translate("Widget", "Queues", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "Messages", Q_NULLPTR));
        label_5->setText(QApplication::translate("Widget", "Users in queue", Q_NULLPTR));
        label_6->setText(QApplication::translate("Widget", "New queue name", Q_NULLPTR));
        privateCheckbox->setText(QApplication::translate("Widget", "private queue", Q_NULLPTR));
        addUserButton->setText(QApplication::translate("Widget", "Add user to queue", Q_NULLPTR));
        refreshButton->setText(QApplication::translate("Widget", "Refresh list", Q_NULLPTR));
        label_7->setText(QApplication::translate("Widget", "User to invite", Q_NULLPTR));
        label_8->setText(QApplication::translate("Widget", "Validity time:", Q_NULLPTR));
        leaveQueueButton->setText(QApplication::translate("Widget", "Leave queue", Q_NULLPTR));
        deleteQueueButton->setText(QApplication::translate("Widget", "Delete queue", Q_NULLPTR));
        loginStatus->setText(QApplication::translate("Widget", "You are not logged in", Q_NULLPTR));
        logoutButton->setText(QApplication::translate("Widget", "Log out", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
