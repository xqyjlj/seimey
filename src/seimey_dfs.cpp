#include "seimey_dfs.h"
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
seimey_dfs::seimey_dfs(QObject *parent) : QObject(parent)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_Timeout()), Qt::UniqueConnection);
}
seimey_dfs::~seimey_dfs()
{

}
/*
 * 文件管理器的控制函数
*/
void seimey_dfs::handle(QString msg)
{
    if ((event & 0x01) == 0x01)
    {
        timer->setInterval(100);
        timer->start();
        msg_list << msg;
    }
}
void seimey_dfs::timer_Timeout()
{
    timer->stop();
    event &= 0xfe;
    bypass(&msg_list);
    msg_list.clear();
}
void seimey_dfs::bypass(QStringList *list)
{
    QString msg = list->at(0);
    if (msg.contains(">cd ") && msg.contains("msh"))
    {
        ctl_ls(list);
    }
    else if (msg.contains(">cat ") && msg.contains("msh"))
    {
        ctl_cat(list);
    }
}
void seimey_dfs::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
}

void seimey_dfs::ctl_ls(QStringList *list)
{
    if (list->size() >= 5)
    {
        QString msg = list->at(0);
        if (msg.indexOf(">cd /") == (msg.length() - QString(">cd /").length()))
        {
            ctl_master(list);
        }
        else if (msg.indexOf(">cd /"))
        {
            ctl_servant(list);
        }
    }
}

void seimey_dfs::ctl_master(QStringList *list)
{
    QString msg = list->at(1);
    if (msg == QString("msh />"))
    {
        msg = list->at(2);
        if (msg == QString("msh />ls"))
        {
            if (list->at(1) == list->last())
            {
                msg = list->at(3);
                if (msg == QString("Directory /:"))
                {
                    for (int j = 4; j < list->size() - 1; j++)
                    {
                        msg = list->at(j);
                        msg = msg.simplified();
                        QStringList list_eu = msg.split(" ");
                        if (list_eu.size() == 2)
                        {
                            if (list_eu.at(1) == QString("<DIR>"))
                            {
                                QTreeWidgetItem *child = new QTreeWidgetItem(enumdir);
                                child->setWhatsThis(0, QString("/"));
                                child->setText(0, list_eu.at(0));
                                child->setIcon(0, QIcon(QString(":/icon/qrc/icon/folder.png")));
                                tree_dfs->addTopLevelItem(child);
                            }
                            else
                            {
                                QTreeWidgetItem *child = new QTreeWidgetItem(enumfile);
                                child->setWhatsThis(0, QString("/"));
                                child->setText(0, list_eu.at(0));
                                child->setText(1, list_eu.at(1));
                                child->setIcon(0, QIcon(QString(":/icon/qrc/icon/file.png")));
                                tree_dfs->addTopLevelItem(child);
                            }
                        }
                    }
                }
            }
        }
    }
}

void seimey_dfs::ctl_servant(QStringList *list)
{
    QString msg = list->at(0);
    QString dir;
    int index = msg.indexOf(">cd /");
    dir = msg.remove(0, index + QString(">cd ").length());
    msg = list->at(1);
    if (msg == (QString("msh ") + dir + QString(">")))
    {
        if (list->at(2) == (list->at(1) + QString("ls")))
        {
            if (list->at(1) == list->last())
            {
                msg = list->at(3);
                if (msg == (QString("Directory ") + dir + QString(":")))
                {
                    for (int j = 4; j < list->size() - 1; j++)
                    {
                        msg = list->at(j);
                        msg = msg.simplified();
                        QStringList list_eu = msg.split(" ");
                        if (list_eu.size() == 2)
                        {

                            if (list_eu.at(1) == QString("<DIR>"))
                            {
                                QTreeWidgetItem *child = new QTreeWidgetItem(enumdir);
                                child->setWhatsThis(0, dir + QString("/"));
                                child->setText(0, list_eu.at(0));
                                child->setIcon(0, QIcon(QString(":/icon/qrc/icon/folder.png")));
                                tree_dfs_item->addChild(child);
                                tree_dfs_item->setExpanded(true);
                            }
                            else
                            {
                                QTreeWidgetItem *child = new QTreeWidgetItem(enumfile);
                                child->setWhatsThis(0, dir + QString("/"));
                                child->setText(0, list_eu.at(0));
                                child->setText(1, list_eu.at(1));
                                child->setIcon(0, QIcon(QString(":/icon/qrc/icon/file.png")));
                                tree_dfs_item->addChild(child);
                                tree_dfs_item->setExpanded(true);
                            }
                        }
                    }
                }
            }
        }
    }
}
void seimey_dfs::master(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data((char *)"cd /\r\nls\r\n");
    tree_dfs = obj;
    tree_dfs->clear();
}
void seimey_dfs::servant(seimey_serial *Serial, QTreeWidgetItem *obj)
{
    QString aims;
    QByteArray aims_byte;
    tree_dfs_item = obj;
    aims = obj->whatsThis(0)  + obj->text(0);
    aims_byte = aims.toLatin1();
    if (tree_dfs_item->text(1) == "")
    {
        QList<QTreeWidgetItem *> Children = tree_dfs_item->takeChildren();
        qDeleteAll(Children.begin(), Children.end());
        Children.clear();
        event |= 0x01;
        Serial->send_data((char *)"cd ");
        Serial->send_data((char *)aims_byte.data());
        Serial->send_data((char *)"\r\nls\r\n");
    }
    else if (tree_dfs_item->text(1).toULongLong() != 0)
    {
        event |= 0x01;
        Serial->send_data((char *)"cat ");
        Serial->send_data((char *)aims_byte.data());
        Serial->send_data((char *)"\r\n");
    }
}
void seimey_dfs::mkdir(seimey_serial *Serial, QString msg, QTreeWidgetItem *obj)
{
    QString aims;
    QByteArray aims_byte;
    tree_dfs_item = obj;
    aims = obj->whatsThis(0)  + obj->text(0) + QString("/") + msg;
    aims_byte = aims.toLatin1();
    Serial->send_data((char *)"mkdir ");
    Serial->send_data((char *)aims_byte.data());
    Serial->send_data((char *)"\r\n");
    sleep(500);
    servant(Serial, obj);
}
void seimey_dfs::echo(seimey_serial *Serial, QString name, QString msg, QTreeWidgetItem *obj)
{
    QString aims;
    QByteArray aims_byte, msg_byte;
    tree_dfs_item = obj;
    aims = obj->whatsThis(0)  + obj->text(0) + QString("/") + name;
    aims_byte = aims.toLatin1();
    msg_byte = msg.toLatin1();
    Serial->send_data((char *)"echo ");
    Serial->send_data((char *)msg_byte.data());
    Serial->send_data((char *)" ");
    Serial->send_data((char *)aims_byte.data());
    Serial->send_data((char *)"\r\n");
    sleep(500);
    servant(Serial, obj);
}
void seimey_dfs::mastermkdir(seimey_serial *Serial, QString msg, QTreeWidget *obj)
{
    QByteArray msg_byte;
    tree_dfs = obj;
    msg_byte = msg.toLatin1();
    Serial->send_data((char *)"mkdir /");
    Serial->send_data((char *)msg_byte.data());
    Serial->send_data((char *)"\r\n");
    sleep(500);
    master(Serial, obj);
}
void seimey_dfs::masterecho(seimey_serial *Serial, QString name, QString msg, QTreeWidget *obj)
{
    QByteArray name_byte, msg_byte;
    tree_dfs = obj;
    name_byte = name.toLatin1();
    msg_byte = msg.toLatin1();
    Serial->send_data((char *)"echo ");
    Serial->send_data((char *)msg_byte.data());
    Serial->send_data((char *)" /");
    Serial->send_data((char *)name_byte.data());
    Serial->send_data((char *)"\r\n");
    sleep(500);
    master(Serial, obj);
}
void seimey_dfs::rm(seimey_serial *Serial, QTreeWidgetItem *obj1, QTreeWidget *obj2)
{
    QString aims;
    QByteArray aims_byte;
    aims = obj1->whatsThis(0)  + obj1->text(0);
    aims_byte = aims.toLatin1();
    Serial->send_data((char *)"rm ");
    Serial->send_data((char *)aims_byte.data());
    Serial->send_data((char *)"\r\n");
    sleep(500);

    if (obj1->parent() != NULL)
    {
        tree_dfs_item = obj1->parent();
        servant(Serial, tree_dfs_item);
    }
    else
    {
        master(Serial, obj2);
    }
}
void seimey_dfs::cp(seimey_serial *Serial, QString src, QString pur, QTreeWidgetItem *obj1, QTreeWidget *obj2)
{
    QByteArray src_byte, pur_byte;
    src_byte = src.toLatin1();
    pur_byte = pur.toLatin1();
    Serial->send_data((char *)"cp ");
    Serial->send_data((char *)src_byte.data());
    Serial->send_data((char *)" ");
    Serial->send_data((char *)pur_byte.data());
    Serial->send_data((char *)"\r\n");
    sleep(500);
    if (obj1->parent() != NULL)
    {
        tree_dfs_item = obj1->parent();
        servant(Serial, tree_dfs_item);
    }
    else
    {
        master(Serial, obj2);
    }
}
void seimey_dfs::mastercp(seimey_serial *Serial, QString src, QTreeWidget *obj)
{
    QByteArray src_byte;
    src_byte = src.toLatin1();
    Serial->send_data((char *)"cp ");
    Serial->send_data((char *)src_byte.data());
    Serial->send_data((char *)" /");
    Serial->send_data((char *)"\r\n");
    sleep(500);
    master(Serial, obj);
}
void seimey_dfs::ctl_cat(QStringList *list)
{
    if (list->size() >= 2)
    {
        QString head, msg;
        msg = list->at(0);
        head = msg.left(msg.indexOf("cat /"));
        if (head.contains("msh") && head.contains(">"))
        {
            msg.clear();
            for (int i = 1 ; i < list->size(); i++)
            {
                msg += list->at(i);
            }
            if (msg.contains(head))
            {
                int index = msg.indexOf(head);
                if (index == msg.length() - head.length())
                {
                    msg = msg.left(index);
                    if (c_Seimey_text == NULL)
                    {
                        c_Seimey_text = new seimey_text();
                        connect(c_Seimey_text, &seimey_text::window_Close, [ = ]()
                        {
                            c_Seimey_text = NULL;
                        });
                        c_Seimey_text->show();
                        c_Seimey_text->show_text(msg);
                    }
                }
            }
        }
    }
}
