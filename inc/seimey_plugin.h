#ifndef SEIMEY_PLUGIN_H
#define SEIMEY_PLUGIN_H

#include <QObject>
#include <QPluginLoader>
#include <QTreeWidgetItem>
#include <QStringList>

#include "gdut_fsae_interface.h"
class seimey_plugin : public QObject
{
    Q_OBJECT
public:
    explicit seimey_plugin(QObject *parent = nullptr);

    void rec_msg(QByteArray msg);/*插件通信口，用来传递主程序到插件的QString变量（通常为串口数据）*/
    void set_plugin(QTreeWidget *obj);
    void open_plugin(QTreeWidgetItem *obj);
signals:
    void send_msg(QByteArray msg);
public slots:

private slots:
    void close_event_collect(QString msg);/* 收集插件的关闭信息，用来将插件指针置为NULL */
    void forward_msg(QByteArray msg);
private:
    void loader_plugin(QObject *obj);
    void loader_gdut_fsae_plugin(QObject *obj);
private:
    gdut_fsae_interface *_pgdut_fsae_interface = NULL;

    QStringList plugin_list =
    {
        "",
        "gdut_fsae_plugin",
    };

    enum enum_plugin
    {
        PLUGIN_NULL = 0,
        PLUGIN_GDUT_FSAE
    };
};

#endif // SEIMEY_PLUGIN_H
