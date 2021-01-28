/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLineEdit *loginField;
    QLineEdit *passwordField;
    QLabel *label;
    QLabel *label_2;
    QLabel *statusMessage;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QStringLiteral("Login"));
        Login->resize(327, 208);
        loginButton = new QPushButton(Login);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        loginButton->setGeometry(QRect(220, 160, 85, 29));
        registerButton = new QPushButton(Login);
        registerButton->setObjectName(QStringLiteral("registerButton"));
        registerButton->setEnabled(true);
        registerButton->setGeometry(QRect(20, 160, 85, 29));
        loginField = new QLineEdit(Login);
        loginField->setObjectName(QStringLiteral("loginField"));
        loginField->setGeometry(QRect(100, 40, 201, 29));
        passwordField = new QLineEdit(Login);
        passwordField->setObjectName(QStringLiteral("passwordField"));
        passwordField->setGeometry(QRect(100, 100, 201, 29));
        passwordField->setEchoMode(QLineEdit::Password);
        label = new QLabel(Login);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(13, 46, 71, 21));
        label_2 = new QLabel(Login);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setEnabled(true);
        label_2->setGeometry(QRect(13, 106, 71, 21));
        statusMessage = new QLabel(Login);
        statusMessage->setObjectName(QStringLiteral("statusMessage"));
        statusMessage->setGeometry(QRect(30, 139, 261, 16));

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "Form", Q_NULLPTR));
        loginButton->setText(QApplication::translate("Login", "Login", Q_NULLPTR));
        registerButton->setText(QApplication::translate("Login", "Register", Q_NULLPTR));
        label->setText(QApplication::translate("Login", "Login", Q_NULLPTR));
        label_2->setText(QApplication::translate("Login", "Password", Q_NULLPTR));
        statusMessage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
