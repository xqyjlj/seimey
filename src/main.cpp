#include "mainwindow.h"
#include <QApplication>
#include "seimey_data.h"
int main(int argc, char *argv[])
{
#ifdef QT_NO_DEBUG
    qInstallMessageHandler(seimey_Msg_Output);
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
