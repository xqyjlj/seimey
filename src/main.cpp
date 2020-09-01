/*
* main函数
*
* 修改日志:
*
* 日期            作者          Notes
* 2020-08-12     xqyjlj       0.0.0     ：创建项目
*  ... ...     (中间日志丢失)
* 2020-08-25     xqyjlj       0.0.8     ：增加日志输出路径转移哦
*/
#include "mainwindow.h"
#include <QApplication>
#include <QMutex>
#include <QDateTime>
#include <QDir>
/*
 * 改变日志输出路径
*/
void msg_Output(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString time = QDateTime::currentDateTime().toString(QString("[ yyyy-MM-dd HH:mm:ss:zzz ]"));
    QString mmsg;
    switch (type)
    {
    case QtDebugMsg:
        mmsg = QString("%1: Debug:\t%2 (file:%3, line:%4, func: %5)").arg(time).arg(msg).arg(QString(context.file)).arg(context.line).arg(QString(context.function));
        break;
    case QtInfoMsg:
        mmsg = QString("%1: Info:\t%2 (file:%3, line:%4, func: %5)").arg(time).arg(msg).arg(QString(context.file)).arg(context.line).arg(QString(context.function));
        break;
    case QtWarningMsg:
        mmsg = QString("%1: Warning:\t%2 (file:%3, line:%4, func: %5)").arg(time).arg(msg).arg(QString(context.file)).arg(context.line).arg(QString(context.function));
        break;
    case QtCriticalMsg:
        mmsg = QString("%1: Critical:\t%2 (file:%3, line:%4, func: %5)").arg(time).arg(msg).arg(QString(context.file)).arg(context.line).arg(QString(context.function));
        break;
    case QtFatalMsg:
        mmsg = QString("%1: Fatal:\t%2 (file:%3, line:%4, func: %5)").arg(time).arg(msg).arg(QString(context.file)).arg(context.line).arg(QString(context.function));
        abort();
    }
    static QString qs_log_file = QDir::currentPath() + "/.workspace" + "/.log" + "/seimey.log";
    QFile file(qs_log_file);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << mmsg << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char *argv[])
{
#ifdef QT_NO_DEBUG
    qInstallMessageHandler(msg_Output);
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
