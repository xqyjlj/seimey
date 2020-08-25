#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "seimey_serial.h"
#include "seimey_qwchse.h"
#include "seimey_setting.h"
#include "seimey_plugin.h"
#include <QTimer>

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
    void serial_data_coming(void);/* 串口数据来临槽函数 */
    void on_tabW_task_manager_tabBarDoubleClicked(int index);/* 任务管理器顶端标签双击槽函数 */
    void on_tabW_task_manager_currentChanged(int index);/* 任务管理器顶端标签改变槽函数 */
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
    void on_menu_plugin_triggered();/* 菜单·插件按键点击槽函数 */
    void on_menu_plugin_2_triggered();/* 菜单·插件按键点击槽函数 */

private:
    void init_ctl(void);/* 初始化界面的控件 */
    void init_connect(void);/* 用来初始化一些connect函数 */
    void assign_finsh_mode(int mode, QStringList *list);/* finsh基本命令的控制分流函数 */
    void finsh_thread(QStringList *list);/* finsh Thread */
    void finsh_device(QStringList *list);/* finsh Device */
    void finsh_timer(QStringList *list);/* finsh Timer */
    void finsh_mempool(QStringList *list);/* finsh Mempool */
    void finsh_memheap(QStringList *list);/* finsh Memheap */
    void finsh_memfree(QStringList *list);/* finsh Memfree */
    void finsh_sem(QStringList *list);/* finsh Sem */
    void finsh_mutex(QStringList *list);/* finsh Mutex */
    void finsh_event(QStringList *list);/* finsh Event */
    void finsh_mailbox(QStringList *list);/* finsh Mailbox */
    void finsh_msgqueue(QStringList *list);/* finsh Msgqueue */
    void assign_button_mem(int mode);/* finsh内存命令的控制分流函数 */
    void assign_button_synchronize(int mode);/* finsh同步命令的控制分流函数 */
    void assign_button_communicate(int mode);/* finsh通信命令的控制分流函数 */
    void task_handle(QString data);/* 任务管理器的控制函数 */
    void dfs_handle(QString data);/* 文件管理器的控制函数 */

private:
    Ui::MainWindow *ui;
    seimey_serial *c_Seimey_Serial = new seimey_serial(this);
    seimey_qwchse *c_Seimey_Qwchse = NULL;
    seimey_setting *c_Seimey_Setting = NULL;

    QTimer *finsh_timerID = NULL;

    struct
    {
        seimey_plugin *seimey_plugin_c = NULL;
        bool isexit = false;
    } plugin;

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

    enum enum_mem_mode
    {
        MEM_HEAP = 0,
        MEM_POOL,
        MEM_FREE
    };

    enum enum_finsh_mode
    {
        RT_NO = 0,
        RT_THREAD,
        RT_SEM,
        RT_EVENT,
        RT_MUTEX,
        RT_MAILBOX,
        RT_MSGQUEUE,
        RT_MEMTOOL,
        RT_MEMHEAP,
        RT_TIMER,
        RT_DEVICE,
        RT_FREE
    };

    enum enum_msh_status
    {
        MSH_NULL = 0,
        MSH_GET_MSH,
        MSH_GET_COMMAND,
        MSH_DOWN
    };

    enum enum_synchronize_mode
    {
        SYNCHR_SEM = 0,
        SYNCHR_MUTEX,
        SYNCHR_EVENT
    };

    enum enum_communicate_mode
    {
        COMMUN_MAIL = 0,
        COMMUN_QUEUE,
    };

    QStringList finsh_mode_list = {"",
                                   "list_thread",
                                   "list_sem",
                                   "list_event",
                                   "list_mutex",
                                   "list_mailbox",
                                   "list_msgqueue",
                                   "list_mempool",
                                   "list_memheap",
                                   "list_timer",
                                   "list_device",
                                   "free"
                                  };

    enum enum_dfs_mode
    {
        DFS_NO = 0,
        DFS_LS,
        DFS_CD,
        DFS_CP,
        DFS_RM,
        DFS_MV,
        DFS_ECHO,
        DFS_CAT,
        DFS_PWD,
        DFS_MKDIR,
        DFS_MKFS
    };

    QStringList dfs_mode_list = {"",
                                 "ls",
                                 "cd",
                                 "cp",
                                 "rm",
                                 "mv",
                                 "echo",
                                 "cat",
                                 "pwd",
                                 "mkdir",
                                 "mkdir"
                                };


};



#endif // MAINWINDOW_H
