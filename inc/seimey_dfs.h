#ifndef SEIMEY_DFS_H
#define SEIMEY_DFS_H

#include "seimey_serial.h"
#include <QObject>
#include <QStringList>
#include <QString>
#include <QTimer>
#include <QQueue>
#include <QTreeWidget>
#include "seimey_text.h"
class seimey_dfs : public QObject
{
    Q_OBJECT
public:
    explicit seimey_dfs(QObject *parent = nullptr);
    ~seimey_dfs();
public:
    void master(seimey_serial *Serial, QTreeWidget *obj);
    void servant(seimey_serial *Serial, QTreeWidgetItem *obj);
    void mkdir(seimey_serial *Serial, QString msg, QTreeWidgetItem *obj);
    void echo(seimey_serial *Serial, QString name, QString msg, QTreeWidgetItem *obj);
    void mastermkdir(seimey_serial *Serial, QString msg, QTreeWidget *obj);
    void masterecho(seimey_serial *Serial, QString name, QString msg, QTreeWidget *obj);
    void rm(seimey_serial *Serial, QTreeWidgetItem *obj1, QTreeWidget *obj2);
    void cp(seimey_serial *Serial, QString src, QString pur, QTreeWidgetItem *obj1, QTreeWidget *obj2);
    void mastercp(seimey_serial *Serial, QString src, QTreeWidget *obj);
    void handle(QString msg);/*  */
signals:

public slots:


private:
    void ctl_ls(QStringList *list);
    void ctl_master(QStringList *list);
    void ctl_servant(QStringList *list);
    void ctl_cat(QStringList *list);
    void sleep(int msec);
    void bypass(QStringList *list);
private:
    uint8_t event;
    QStringList msg_list;
    QTimer *timer = new QTimer(this);
    QTreeWidget *tree_dfs;
    QTreeWidgetItem *tree_dfs_item;
    seimey_text *c_Seimey_text = NULL;
public:
    enum itemType {enumdir = QTreeWidgetItem::UserType, enumfile};
private slots:
    void timer_Timeout(void);
};

#endif // SEIMEY_DFS_H
