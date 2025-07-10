#include <QApplication>

#include "gui/mainwindow.h"
#include "core/User.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<User>("User");

    QApplication a(argc, argv);


    MainWindow w;
    w.show();

    return a.exec();
}
