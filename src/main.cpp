#include "mainwindow.h"
#include <QApplication>
#include "seimey_data.h"
int main(int argc, char *argv[])
{
//    qInstallMessageHandler(seimey_msgoutput);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
