#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "seimey_serial.h"
#include "seimey_qwchse.h"
#include "seimey_setting.h"


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
    void on_menu_setting_serial_setting_triggered(void);
    void on_menu_setting_serial_link_triggered(void);
    void on_menu_help_about_triggered(void);
    void on_menu_help_github_triggered(void);
    void on_menu_setting_preference_triggered(void);
    void on_menu_setting_serial_setting_2_triggered(void);
    void serial_data_coming(void);
    void on_tabW_task_manager_tabBarDoubleClicked(int index);
    void on_tabW_task_manager_currentChanged(int index);
    void on_comdLB_mem_pool_clicked(void);
    void on_comdLB_mem_heap_clicked(void);
    void on_comdLB_mem_free_clicked(void);
    void on_comdLB_synchronize_semaphore_clicked();
    void on_comdLB_synchronize_mutex_clicked();
    void on_comdLB_synchronize_event_clicked();
    void on_comdLB_communicate_mail_clicked();
    void on_comdLB_communicate_queue_clicked();

private:
    Ui::MainWindow *ui;
    seimey_serial *seimey_serial_c = new seimey_serial(this);
    seimey_qwchse *seimey_qwchse_c;
    seimey_setting *seimey_setting_c;

    void init_ctl(void);
    void init_connect(void);
    void assign_finsh_mode(int mode, QStringList *list);
    void finsh_thread(QStringList *list);
    void finsh_device(QStringList *list);
    void finsh_timer(QStringList *list);
    void finsh_mempool(QStringList *list);
    void finsh_memheap(QStringList *list);
    void finsh_memfree(QStringList *list);
    void finsh_sem(QStringList *list);
    void finsh_mutex(QStringList *list);
    void finsh_event(QStringList *list);
    void finsh_mailbox(QStringList *list);
    void finsh_msgqueue(QStringList *list);
    void assign_button_mem(int mode);
    void assign_button_synchronize(int mode);
    void assign_button_communicate(int mode);
};
#endif // MAINWINDOW_H
