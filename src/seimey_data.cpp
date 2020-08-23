#include "seimey_data.h"
#include <QString>
#include <QDir>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
static void ceate_serial(void)
{

    QString qs_serial_dir = QDir::currentPath() + "/.workspace" + "/.serial";

    QDir serial_dir(qs_serial_dir);
    if (!serial_dir.exists())
    {
        serial_dir.mkdir(qs_serial_dir);
    }

    QString qs_serial_set_file = qs_serial_dir + "/serial.json";
    QFile serial_set_file(qs_serial_set_file);
    if (!serial_set_file.exists())
    {
        serial_set_file.open(QIODevice::Append | QIODevice::Text);
        unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        serial_set_file.write((char *)bom, sizeof(bom));
        QTextStream in(&serial_set_file);
        in.setCodec("UTF-8");
        in << "{\n";
        in << "    \"SerialPort\": \"\",\n";
        in << "    \"BaudRate\": \"\",\n";
        in << "    \"DataBits\": \"\",\n";
        in << "    \"FlowControl\": \"\",\n";
        in << "    \"Parity\": \"\",\n";
        in << "    \"StopBits\": \"\"\n";
        in << "}";
        serial_set_file.flush();
        serial_set_file.close();
    }

    QString qs_serial_data_file = qs_serial_dir + "/serial.txt";
    QFile serial_data_file(qs_serial_data_file);
    if (!serial_data_file.exists())
    {
        serial_data_file.open(QIODevice::Append | QIODevice::Text);
        unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        serial_data_file.write((char *)bom, sizeof(bom));
        QTextStream in(&serial_data_file);
        in.setCodec("UTF-8");
        in << QString("# 串口数据\n");
        serial_data_file.flush();
        serial_data_file.close();
    }
}


static void ceate_log(void)
{

    QString qs_log_dir = QDir::currentPath() + "/.workspace" + "/.log";
    QDir log_dir(qs_log_dir);
    if (!log_dir.exists())
    {
        log_dir.mkdir(qs_log_dir);
    }

    QString qs_log_file = qs_log_dir + "/seimey.log";
    QFile log_file(qs_log_file);
    if (!log_file.exists())
    {
        log_file.open(QIODevice::Append | QIODevice::Text);
        unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        log_file.write((char *)bom, sizeof(bom));
        log_file.flush();
        log_file.close();
    }
}


static void ceate_setting(void)
{

    QString qs_set_dir = QDir::currentPath() + "/.workspace" + "/.setting";
    QDir set_dir(qs_set_dir);
    if (!set_dir.exists())
    {
        set_dir.mkdir(qs_set_dir);
    }

    QString qs_setting_file = qs_set_dir + "/setting.json";
    QFile setting_file(qs_setting_file);
    if (!setting_file.exists())
    {
        setting_file.open(QIODevice::Append | QIODevice::Text);
        unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        setting_file.write((char *)bom, sizeof(bom));
        QTextStream in(&setting_file);
        in.setCodec("UTF-8");
        in << "{\n";
        in << "    \"Save serial\": false,\n";
        in << "    \"Timed refresh\": 0\n";
        in << "}";
        setting_file.flush();
        setting_file.close();
    }
}

void seimey_create_workspace(void)
{
    /* create workspace folder */
    QString qs_workspace_dir = QDir::currentPath() + "/.workspace";
    QDir workspace_dir(qs_workspace_dir);
    if (!workspace_dir.exists())
    {
        workspace_dir.mkdir(qs_workspace_dir);
    }
    ceate_serial();
    ceate_log();
    ceate_setting();
}


void seimey_msgoutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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

bool seimey_get_is_save_serial_data(void)
{
    QJsonParseError jsonError;

    bool is_save_serial_data = false;
    QString qs_setting_file = QDir::currentPath() + "/.workspace/.setting/setting.json";
    QFile setting_file(qs_setting_file);
    setting_file.open(QIODevice::ReadOnly);

    QJsonDocument setting_json = QJsonDocument::fromJson(setting_file.readAll(), &jsonError);
    if (!setting_json.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject setting_json_obj = setting_json.object();
        is_save_serial_data = setting_json_obj.value("Save serial").toBool();
    }

    setting_file.close();
    return is_save_serial_data;
}

double seimey_get_timed_refresh_time(void)
{
    QJsonParseError jsonError;

    double timed_refresh_time = 0;
    QString qs_setting_file = QDir::currentPath() + "/.workspace/.setting/setting.json";
    QFile setting_file(qs_setting_file);
    setting_file.open(QIODevice::ReadOnly);

    QJsonDocument setting_json = QJsonDocument::fromJson(setting_file.readAll(), &jsonError);
    if (!setting_json.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject setting_json_obj = setting_json.object();
        timed_refresh_time = setting_json_obj.value("Timed refresh").toDouble();
    }

    setting_file.close();
    return timed_refresh_time;
}
