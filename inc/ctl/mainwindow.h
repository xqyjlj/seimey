/*
* 主窗口
*
* 修改日志:
*
* 日期            作者          Notes
* 2020-08-12     xqyjlj       0.0.0     ：创建项目
* 2020-08-13     xqyjlj       0.0.2     ：创建菜单及其基本控件
*  ... ...     (中间日志丢失)
* 2020-08-25     xqyjlj       0.0.8     ：创建插件管理器
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "seimey_serial.h"
#include "seimey_qwchse.h"
#include "seimey_setting.h"
#include "seimey_plugin.h"
#include "seimey_data.h"
#include "seimey_finsh.h"
#include "seimey_dfs.h"
#include <QTimer>
#include <QMenu>
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_menu_setting_serial_setting_triggered(void);/* 菜单·串口设置按键点击槽函数 */
    void on_menu_setting_serial_link_triggered(void);/* 菜单·串口连接按键点击槽函数 */
    void on_menu_help_about_triggered(void);/* 菜单·帮助·关于按键点击槽函数 */
    void on_menu_help_github_triggered(void);/* 菜单·帮助·github地址按键点击槽函数 */
    void on_menu_setting_preference_triggered(void);/* 菜单·首选项设置按键点击槽函数 */
    void on_menu_setting_serial_setting_2_triggered(void);/* 菜单·串口设置按键点击槽函数 */
    void serial_data_coming(QByteArray msg);/* 串口数据来临槽函数 */
    void on_comdLB_mem_pool_clicked(void);/* 内存池按钮点击槽函数 */
    void on_comdLB_mem_heap_clicked(void);/* 内存堆按钮点击槽函数 */
    void on_comdLB_mem_free_clicked(void);/* 动态按钮点击槽函数 */
    void on_comdLB_synchronize_semaphore_clicked();/* 信号量按钮点击槽函数 */
    void on_comdLB_synchronize_mutex_clicked();/* 二值信号量按钮点击槽函数 */
    void on_comdLB_synchronize_event_clicked();/* 事件按钮点击槽函数 */
    void on_comdLB_communicate_mail_clicked();/* 邮箱按钮点击槽函数 */
    void on_comdLB_communicate_queue_clicked();/* 队列按钮点击槽函数 */
    void on_menu_setting_preference_2_triggered();/* 菜单·首选项设置按键点击槽函数 */
    void finsh_timer_timeout();/* finsh定时刷新触发的槽函数 */
    void on_tabW_main_currentChanged(int index);
    void on_tabW_main_tabBarDoubleClicked(int index);
    void on_treeW_file_mannage_itemDoubleClicked(QTreeWidgetItem *item);
    void showTreeRightMenu(QPoint pos);
    void on_dfs_menu_open_triggered();
    void on_dfs_menu_clone_triggered();
    void on_dfs_menu_mkdir_triggered();
    void on_dfs_menu_echo_triggered();
    void on_dfs_menu_rm_triggered();
    void on_dfs_menu_copy_triggered();
    void on_dfs_menu_paste_triggered();
    void on_dfs_menu_clone_2_triggered();
    void on_dfs_menu_mkdir_2_triggered();
    void on_dfs_menu_echo_2_triggered();
    void on_dfs_menu_paste_2_triggered();
    void on_treeW_plugin_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void send_msg(QByteArray msg);

private:
    void init_ctl(void);/* 初始化界面的控件 */
    void init_connect(void);/* 用来初始化一些connect函数 */
    void task_manager(int index);/* 任务管理器顶端标签改变槽函数 */

private:
    Ui::MainWindow *ui;
    seimey_serial *c_Seimey_Serial = new seimey_serial(this);
    seimey_qwchse *c_Seimey_Qwchse = NULL;
    seimey_setting *c_Seimey_Setting = NULL;
    seimey_plugin *c_Seimey_Plugin = new seimey_plugin(this);
    QTimer *finsh_timerID = new QTimer(this);;
    seimey_data *c_Seimey_Data = new seimey_data(this);
    seimey_finsh *c_Seimey_Finsh = new seimey_finsh(this);
    seimey_dfs *c_Seimey_Dfs = new seimey_dfs(this);
    QMenu *dfs_menu = new QMenu(this);
    QTreeWidgetItem *treew_dfs_MenuRequested = NULL;
    QString dfs_clipboard;
    struct
    {
        QLabel *port_status;
        QLabel *baud_status;
        QLabel *status;
    } serial_status;

    struct
    {
        bool is_save_serial;
        double timed_refresh;
    } setting;
};



#endif // MAINWINDOW_H
