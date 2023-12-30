#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LidarHMI w;
    w.show();
    return a.exec();
}
