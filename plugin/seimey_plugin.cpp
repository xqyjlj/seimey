#include "seimey_plugin.h"
#include "ui_seimey_plugin.h"
#include <QDir>
#include <QDebug>
seimey_plugin::seimey_plugin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::seimey_plugin)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    set_Plugin();
    setAttribute(Qt::WA_QuitOnClose, false);
}

seimey_plugin::~seimey_plugin()
{
    delete ui;
}
/*
 * 设置插件列表：将存在于.plugin文件里面的插件呈现于QTreeWidget中
*/
void seimey_plugin::set_Plugin(void)
{
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

            ui->treeW_plugin->addTopLevelItem(child);
        }
    }
}
/*
 * QT槽函数，当插件QTreeWidget的节点被双击后，触发此函数
*/
void seimey_plugin::on_treeW_plugin_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (column >= 0)
    {
        QString qs_plugin_path = QDir::currentPath() + "/.plugin/" + item->text(0) + ".dll";

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
        open_Plugin(pPlugin);
    }
}
/*
 * 插件通信口，用来传递主程序到插件的QString变量（通常为串口数据）
*/
void seimey_plugin::rec_msg(QString msg)
{
    if (_pgdut_fsae_interface != NULL)
    {
        _pgdut_fsae_interface->rec_Msg(msg);
    }
}
/*
 * 插件管理器窗口的关闭事件，用来发送窗口关闭的信号
*/
void seimey_plugin::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit window_Close();
}
/*
 * 打开插件
*/
void seimey_plugin::open_Plugin(QObject *obj)
{
    if (obj)
    {
        QString pluginName = obj->metaObject()->className();/* 获取插件的类名 */
        int index = plugin_list.indexOf(pluginName);/* 获取插件类名在插件列表的位置 */
        if (index >= 0)
        {
            switch (index)
            {
            /* GDUT_FSAE 插件 */
            case PLUGIN_GDUT_FSAE:
                if (_pgdut_fsae_interface == NULL)
                {
                    _pgdut_fsae_interface = qobject_cast<gdut_fsae_interface *>(obj);
                    if (_pgdut_fsae_interface != NULL)
                    {
                        connect(obj, SIGNAL(plugin_Close(QString)), this, SLOT(close_Event_Collect(QString)), Qt::UniqueConnection);
                        _pgdut_fsae_interface->show();
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}
/*
 * 收集插件的关闭信息，用来将插件指针置为NULL
*/
void seimey_plugin::close_Event_Collect(QString msg)
{
    int index = plugin_list.indexOf(msg);
    if (index >= 0)
    {
        switch (index)
        {
        case PLUGIN_GDUT_FSAE:
            _pgdut_fsae_interface = NULL;
            break;
        }
    }
}
