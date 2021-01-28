#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    ServerConnection serv("../config/connection.json");
    MessageHandler handler(&serv);
    handler.loop();

    QApplication a(argc, argv);
    Widget w(nullptr, &serv, &handler);
    w.show();

    return a.exec();
}
