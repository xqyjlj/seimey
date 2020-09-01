/*
* 插件管理器
*
* 修改日志:
*
* 日期            作者          Notes
* 2020-08-22     xqyjlj       0.0.0     ：添加插件管理器，
* 2020-08-25     xqyjlj       0.0.1     ：修改函数，启用信号槽来进行插件和插件管理器的通信
*/
#ifndef SEIMEY_PLUGIN_H
#define SEIMEY_PLUGIN_H

#include <QMainWindow>
#include "gdut_fsae_interface.h"
#include <QTreeWidgetItem>
#include <QPluginLoader>
#include <QMouseEvent>
namespace Ui
{
class seimey_plugin;
}

class seimey_plugin : public QMainWindow
{
    Q_OBJECT
public:
    explicit seimey_plugin(QWidget *parent = nullptr);
    ~seimey_plugin();

    void rec_msg(QString msg);/*插件通信口，用来传递主程序到插件的QString变量（通常为串口数据）*/

protected:
    virtual void closeEvent(QCloseEvent *event);/* 插件管理器窗口的关闭事件，用来发送窗口关闭的信号 */

signals:
    void window_Close(void);/* 插件管理器窗口的关闭信号 */

private slots:
    void on_treeW_plugin_itemDoubleClicked(QTreeWidgetItem *item, int column);/* QT槽函数，当插件QTreeWidget的节点被双击后，触发此函数 */
    void close_Event_Collect(QString msg);/* 收集插件的关闭信息，用来将插件指针置为NULL */

private:
    void set_Plugin(void);/* 设置插件列表：将存在于.plugin文件里面的插件呈现于QTreeWidget中 */
    void open_Plugin(QObject *obj);/* 打开插件 */

private:
    Ui::seimey_plugin *ui;
    gdut_fsae_interface *_pgdut_fsae_interface = NULL;

    QStringList plugin_list =
    {
        "",
        "gdut_fsae_plugin"
    };

    enum enum_plugin
    {
        PLUGIN_NULL,
        PLUGIN_GDUT_FSAE
    };
};

#endif // SEIMEY_PLUGIN_H
