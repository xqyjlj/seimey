/*
* 数据处理
*
* 修改日志:
*
* 日期            作者          Notes
* 2020-08-17     xqyjlj       0.0.0     ：创建项目
*  ... ...     (中间日志丢失)
* 2020-08-25     xqyjlj       0.0.8     ：创建插件管理器
*/
#include "seimey_data.h"
#include <QString>
#include <QDir>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

seimey_data::seimey_data(QObject *parent) : QObject(parent)
{

}
/*
 * 创建串口工作目录
*/
void seimey_data::ceate_Serial(void)
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
/*
 * 创建日志工作目录
*/
void seimey_data::ceate_Log(void)
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
/*
 * 创建设置工作目录
*/
void seimey_data::ceate_Setting(void)
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
        in << "    \"Timed refresh\": 0.5\n";
        in << "}";
        setting_file.flush();
        setting_file.close();
    }
}
/*
 * 创建工作目录
*/
void seimey_data::create_Workspace(void)
{
    /* create workspace folder */
    QString qs_workspace_dir = QDir::currentPath() + "/.workspace";
    QDir workspace_dir(qs_workspace_dir);
    if (!workspace_dir.exists())
    {
        workspace_dir.mkdir(qs_workspace_dir);
    }
    ceate_Serial();
    ceate_Log();
    ceate_Setting();
}
/*
 * 创建插件工作目录
*/
void create_Plugin(void)
{
    QString qs_plugin_dir = QDir::currentPath() + "/.plugin";
    QDir plugin_dir(qs_plugin_dir);
    if (!plugin_dir.exists())
    {
        plugin_dir.mkdir(qs_plugin_dir);
    }
}
/*
 * 创建目录
*/
void seimey_data::create_Dir(void)
{
    create_Workspace();
    create_Plugin();
}
/*
 * 获得是否保存串口数据的标志位
*/
bool seimey_data::get_Is_Save_Serial_Data(void)
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
/*
 * 获得定时器刷新时间
*/
double seimey_data::get_Timed_Refresh_Time(void)
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
/*
 * 保存串口数据
*/
void seimey_data::save_Serial_Data(QByteArray msg)
{
    static QString qs_serial_data_file = QDir::currentPath() + "/.workspace" + "/.serial" + "/serial.txt";
    QString time = QDateTime::currentDateTime().toString(QString("[ yyyy-MM-dd HH:mm:ss:zzz ]"));
    QFile file(qs_serial_data_file);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << (time + QString(" "));
    stream << msg;
    stream << QString("\r\n");
    file.flush();
    file.close();
}
