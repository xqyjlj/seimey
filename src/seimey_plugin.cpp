#include "seimey_plugin.h"
#include <QDir>
#include <QDebug>
seimey_plugin::seimey_plugin(QObject *parent) : QObject(parent)
{

}

void seimey_plugin::set_plugin(QTreeWidget *obj)
{
    obj->clear();
    QString qs_plugin_dir = QDir::currentPath() + "/.plugin";
    QDir plugin_dir(qs_plugin_dir);
    QFileInfoList fileInfoList = plugin_dir.entryInfoList(QDir::Files);/* 获取文件列表 */
    for (int i = 0; i < fileInfoList.size(); i++)
    {
        QFileInfo fileInfo = fileInfoList.at(i);
        if (fileInfo.suffix() == "dll")/* 判断文件后缀 */
        {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            QString icon = ":/icon/qrc/icon/plugin_" + QString::number(i % 3) + ".png";
            /* 设置节点信息 */
            child->setText(0, fileInfo.baseName());
            child->setIcon(0, QIcon(icon));
            obj->addTopLevelItem(child);
        }
    }
}

void seimey_plugin::rec_msg(QByteArray msg)
{
    if (_pgdut_fsae_interface != nullptr)
    {
        _pgdut_fsae_interface->rec_msg(msg);
    }
}


void seimey_plugin::open_plugin(QTreeWidgetItem *obj)
{
    QString qs_plugin_path = QDir::currentPath() + "/.plugin/" + obj->text(0) + ".dll";

    if (!QFile::exists(qs_plugin_path))
    {
        /* 如果插件不存在，将退出 */
        QByteArray qb_plugin_path = qs_plugin_path.toLatin1();
        qDebug() << "Not exist file: " << qb_plugin_path.data();
        return;
    }

    QPluginLoader pluginLoader(qs_plugin_path);/* 插件对象 */
    pluginLoader.load();/* 加载插件 */
    QObject *pPlugin = pluginLoader.instance();/* 获取插件的根组件对象 */
    loader_plugin(pPlugin);
}

void seimey_plugin::loader_plugin(QObject *obj)
{
    if (obj)
    {
        QString pluginName = obj->metaObject()->className();/* 获取插件的类名 */
        int index = plugin_list.indexOf(pluginName);/* 获取插件类名在插件列表的位置 */
        if (index >= 0)
        {
            switch (index)
            {
            case PLUGIN_GDUT_FSAE:
                loader_gdut_fsae_plugin(obj);
                break;
            default:
                break;
            }
        }
    }
}
void seimey_plugin::loader_gdut_fsae_plugin(QObject *obj)
{
    if (_pgdut_fsae_interface == nullptr)
    {
        _pgdut_fsae_interface = qobject_cast<gdut_fsae_interface *>(obj);
        if (_pgdut_fsae_interface != nullptr)
        {
            connect(obj, SIGNAL(plugin_close(QString)), this, SLOT(close_event_collect(QString)), Qt::UniqueConnection);
            connect(obj, SIGNAL(send_msg(QByteArray)), this, SLOT(forward_msg(QByteArray)), Qt::UniqueConnection);
            _pgdut_fsae_interface->show();
        }
    }
}
void seimey_plugin::close_event_collect(QString msg)
{
    int index = plugin_list.indexOf(msg);
    if (index >= 0)
    {
        switch (index)
        {
        case PLUGIN_GDUT_FSAE:
            _pgdut_fsae_interface = nullptr;
            break;
        }
    }
}


void seimey_plugin::forward_msg(QByteArray msg)
{
    qDebug() << "1" << msg;
    emit send_msg(msg);
}
