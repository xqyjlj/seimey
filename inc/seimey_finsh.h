/*
* finsh基础命令
*
* 修改日志:
*
* 日期            作者          Notes
* 2020-08-26     xqyjlj       0.0.0     ：创建项目
*/
#ifndef SEIMEY_FINSH_H
#define SEIMEY_FINSH_H
#include <QObject>
#include <QTreeWidget>
#include <QStringList>
#include <QString>
#include <QLineEdit>
#include "progresswater.h"
#include <QLabel>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QTimer>
#include <QLineEdit>

#include "seimey_serial.h"
class seimey_finsh : public QObject
{
    Q_OBJECT
public:
    explicit seimey_finsh(QObject *parent = nullptr);
public:
    void handle(QString msg);/* 任务管理器的控制函数 */
    void thread(seimey_serial *Serial, QTableWidget *obj); /* finsh Thread */
    void device(seimey_serial *Serial, QTreeWidget *obj); /* finsh Device */
    void time(seimey_serial *Serial, QTreeWidget *obj, QLineEdit *line);
    void mem_pool(seimey_serial *Serial, QTreeWidget *obj);
    void mem_heap(seimey_serial *Serial, QTreeWidget *obj);
    void mem_free(seimey_serial *Serial, ProgressWater *obj, QLabel *total, QLabel *max, QLabel *current);
    void synchr_sem(seimey_serial *Serial, QTreeWidget *obj);
    void synchr_mutex(seimey_serial *Serial, QTreeWidget *obj);
    void synchr_event(seimey_serial *Serial, QTreeWidget *obj);
    void commun_mail(seimey_serial *Serial, QTreeWidget *obj);
    void commun_queue(seimey_serial *Serial, QTreeWidget *obj);

signals:

public slots:
    void widget_sort(int index);

private:
    void sleep(int msec);
    void bypass(QStringList *list);
    void ctl_thread(QStringList *list);
    void ctl_device(QStringList *list);
    void ctl_timer(QStringList *list);
    void ctl_mem_pool(QStringList *list);
    void ctl_mem_heap(QStringList *list);
    void ctl_mem_free(QStringList *list);
    void ctl_synchr_sem(QStringList *list);
    void ctl_synchr_mutex(QStringList *list);
    void ctl_synchr_event(QStringList *list);
    void ctl_commun_mail(QStringList *list);
    void ctl_commun_queue(QStringList *list);
private:
    uint8_t event;
    QTimer *timer = new QTimer(this);
    QStringList msg_list;
    QTableWidget *tree_thread;
    QTreeWidget *tree_device;
    QTreeWidget *tree_timer;
    QTreeWidget *tree_mem_pool;
    QTreeWidget *tree_mem_heap;
    QTreeWidget *tree_synchr_sem;
    QTreeWidget *tree_synchr_mutex;
    QTreeWidget *tree_synchr_event;
    QTreeWidget *tree_commun_mail;
    QTreeWidget *tree_commun_queue;
    QLineEdit *line_timer;
    ProgressWater *progresswater;
    QLabel *label_total;
    QLabel *label_max;
    QLabel *label_current;

private slots:
    void timer_Timeout(void);
};

#endif // SEIMEY_FINSH_H
