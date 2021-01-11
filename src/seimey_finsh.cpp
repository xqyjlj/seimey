#include "seimey_finsh.h"
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
seimey_finsh::seimey_finsh(QObject *parent) : QObject(parent)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_Timeout()), Qt::UniqueConnection);//定时器超时链接函数
}
void seimey_finsh::handle(QString msg)/* 任务管理器的控制函数 */
{
    if ((event & 0x01) == 0x01)
    {
        timer->setInterval(100); //设置定时器的超时事件是 100ms
        timer->start();
        msg_list << msg; //插入 msg
    }
}

//定时器超时函数
void seimey_finsh::timer_Timeout()
{
    timer->stop();
    event &= 0xfe;

    bypass(&msg_list);

    msg_list.clear();//清空 msg_list
}

//对 list 进行处理
void seimey_finsh::bypass(QStringList *list)
{
    //这个函数只对 size = 3 和 4 进行了处理 todo
    if (list->size() == 3)
    {
        if (list->at(1).contains(QString("command not found.")))
        {
            return;
        }
    }
    if (list->size() >= 4)
    {
        QString msg = list->at(0);// 找到list 中的第一个字符串
        //必须同时包含两个字符串
        if (msg.contains(">list_thread") && msg.contains("msh "))
        {
            ctl_thread(list);
        }
        else if (msg.contains(">list_device") && msg.contains("msh "))
        {
            ctl_device(list);
        }
        else if (msg.contains(">list_timer") && msg.contains("msh "))
        {
            ctl_timer(list);
        }
        else if (msg.contains(">list_mempool") && msg.contains("msh "))
        {
            ctl_mem_pool(list);
        }
        else if (msg.contains(">list_memheap") && msg.contains("msh "))
        {
            ctl_mem_heap(list);
        }
        else if (msg.contains(">free") && msg.contains("msh "))
        {
            ctl_mem_free(list);
        }
        else if (msg.contains(">list_sem") && msg.contains("msh "))
        {
            ctl_synchr_sem(list);
        }
        else if (msg.contains(">list_mutex") && msg.contains("msh "))
        {
            ctl_synchr_mutex(list);
        }
        else if (msg.contains(">list_event") && msg.contains("msh "))
        {
            ctl_synchr_event(list);
        }
        else if (msg.contains(">list_mailbox") && msg.contains("msh "))
        {
            ctl_commun_mail(list);
        }
    }
}
/*
 * finsh Thread
*/
void seimey_finsh::thread(seimey_serial *Serial, QTableWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_thread\r\n");
    tree_thread = obj;
}

void seimey_finsh::widget_sort(int index)
{
    qDebug() << index;
}

bool LessThan(const QString &s1, const QString &s2)
{
    QString s_s1, s_s2;
    QStringList l_s1,l_s2;

    s_s1 = s1.simplified();
    l_s1 = s_s1.split(QString(" "));

    s_s2 = s2.simplified();
    l_s2 = s_s2.split(QString(" "));

    if(l_s1.at(1).toInt() < l_s2.at(1).toInt()) return true;
    else
        return false;
}

void seimey_finsh::ctl_thread(QStringList *list)
{
    QString head, msg;
    QStringList f_list;

    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_thread").length());
    msg = list->last(); //列表中的最后一个字符串

    for(int a = 3; a < list->size() - 1; a++) {
        f_list.append(list->at(a));
    }

    std::sort(f_list.begin(),f_list.end(), LessThan);

    if (msg == head)  //第一个和最后一个 都是 "msh >" 表示报文接收完毕
    {
        int index = 0;

        tree_thread->setRowCount(list->size() - 4);

        for (int i = 0; i < list->size() - 4; i++)
        {
            msg = f_list.at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));

            if (list_eu.size() >= 8)
            {
                QTableWidgetItem *child = tree_thread->item(index, 0);
                if (!child) {
                    child = new QTableWidgetItem();
                    tree_thread->setItem(index, 0, child);
                }
                QString Icon = QString(":/icon/qrc/icon/thread_obj_") + QString::number(i % 3) + QString(".png");

                child->setIcon(QIcon(Icon));
                child->setText(list_eu.at(0));

                for(int j = 1; j < 8; j++)
                {
                    QTableWidgetItem *child_Item = tree_thread->item(index, j);
                    if (!child_Item) {
                        child_Item = new QTableWidgetItem();
                        tree_thread->setItem(index, j, child_Item);
                    }
                    child_Item->setText(list_eu.at(j));
                    child_Item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                }

                index++;
            }
        }
    }
}
/*
 * finsh Device
*/
void seimey_finsh::device(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_device\r\n");
    tree_device = obj;
}
void seimey_finsh::ctl_device(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_device").length());
    msg = list->last();
    if (msg == head)
    {
        tree_device->clear();
        for (int i = 3; i < list->size(); i++)
        {
            msg = list->at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));

            QTreeWidgetItem *child = tree_device->currentItem();
            if(!child){
                child = new QTreeWidgetItem();
            }
            QString Icon = QString(":/icon/qrc/icon/device_") + QString::number(i % 3) + QString(".png");
            child->setIcon(0, QIcon(Icon));
            if (list_eu.size() == 4)
            {
                child->setText(2, list_eu.at(3));
                child->setText(1, list_eu.at(1) + QString(" ") + list_eu.at(2));
                child->setText(0, list_eu.at(0));
                tree_device->addTopLevelItem(child);
            }
            if (list_eu.size() == 3)
            {
                for (int j = 2; j >= 0; j--)
                {
                    child->setText(j, list_eu.at(j));
                }
                tree_device->addTopLevelItem(child);
            }
        }
    }
}
void seimey_finsh::time(seimey_serial *Serial, QTreeWidget *obj, QLineEdit *line)
{
    event |= 0x01;
    Serial->send_data("list_timer\r\n");
    tree_timer = obj;
    line_timer = line;
}
void seimey_finsh::ctl_timer(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_timer").length());
    msg = list->last();
    if (msg == head)
    {
        tree_timer->clear();
        line_timer->setText(QString("********"));
        for (int i = 3; i < list->size() - 2; i++)
        {
            msg = list->at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));
            if (list_eu.size() >= 4)
            {
                QTreeWidgetItem *child = tree_timer->currentItem();
                if(!child){
                    child = new QTreeWidgetItem();
                }
                QString Icon = QString(":/icon/qrc/icon/timer_") + QString::number(i % 3) + QString(".png");
                child->setIcon(0, QIcon(Icon));
                int index = 3;
                for (int j = list_eu.size() - 1; j >= list_eu.size() - 4; j--)
                {
                    child->setText(index--, list_eu.at(j));
                }
                tree_timer->addTopLevelItem(child);
            }
        }
        msg = list->at(list->size() - 2);
        if (msg.contains(QString("current tick:")))
        {
            msg = msg.remove(0, 13);
            line_timer->setText(msg);
        }
    }
}
void seimey_finsh::mem_pool(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_mempool\r\n");
    tree_mem_pool = obj;
}
void seimey_finsh::ctl_mem_pool(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_mempool").length());
    msg = list->last();
    if (msg == head)
    {
        tree_mem_pool->clear();
        for (int i = 3; i < list->size() - 1; i++)
        {
            msg = list->at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));
            if (list_eu.size() >= 5)
            {
                int index = 4;
                QTreeWidgetItem *child = tree_mem_pool->currentItem();
                if(!child){
                    child = new QTreeWidgetItem();
                }
                for (int j = list_eu.size() - 1; j >= list_eu.size() - 5; j--)
                {
                    child->setText(index--, list_eu.at(j));
                }
                tree_mem_pool->addTopLevelItem(child);
            }
        }
    }
}
void seimey_finsh::mem_heap(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_memheap\r\n");
    tree_mem_heap = obj;
}
void seimey_finsh::ctl_mem_heap(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_memheap").length());
    msg = list->last();
    if (msg == head)
    {
        if (list->at(2).contains("----"))
        {
            tree_mem_heap->clear();
            for (int i = 3; i < list->size() - 1; i++)
            {
                msg = list->at(i);
                msg = msg.simplified();
                QStringList list_eu = msg.split(QString(" "));
                if (list_eu.size() >= 4)
                {
                    int index = 3;
                    QTreeWidgetItem *child = tree_mem_heap->currentItem();
                    if(!child){
                        child = new QTreeWidgetItem();
                    }
                    for (int j = list_eu.size() - 1; j >= list_eu.size() - 4; j--)
                    {
                        child->setText(index--, list_eu.at(j));
                    }
                    tree_mem_heap->addTopLevelItem(child);
                }
            }
        }
    }
}
void seimey_finsh::mem_free(seimey_serial *Serial, ProgressWater *obj, QLabel *total, QLabel *max, QLabel *current)
{
    event |= 0x01;
    Serial->send_data("free\r\n");
    progresswater = obj;
    label_total = total;
    label_max = max;
    label_current = current;
}

void seimey_finsh::ctl_mem_free(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("free").length());
    msg = list->last();
    uint64_t max = 0;
    uint64_t total = 0;
    uint64_t current = 0;
    int pro = 0;
    if (msg == head)
    {
        progresswater->setValue(0);
        label_max->setText(QString("*******"));
        label_total->setText(QString("*******"));
        label_current->setText(QString("*******"));
        if (list->at(1).contains(QString("total memory:")))
        {
            if (list->size() == 5)
            {
                msg = list->at(1);
                if (msg.contains("total memory: "))
                {
                    msg = msg.remove(0, QString("total memory: ").length());
                    total = static_cast<uint64_t>(msg.toLongLong());
                }
                msg = list->at(2);
                if (msg.contains("used memory : "))
                {
                    msg = msg.remove(0, QString("used memory : ").length());
                    current = total - msg.toULongLong();
                }
                msg = list->at(3);
                if (msg.contains("maximum allocated memory: "))
                {
                    msg = msg.remove(0, QString("maximum allocated memory: ").length());
                    max = msg.toULongLong();
                }
            }
            if (total == 0)
            {
                pro = 0;
            }
            else
            {
                pro = 100 - static_cast<int>((current * 100) / total);
            }
            if (progresswater->getValue() != pro)
            {
                progresswater->setValue(pro);
            }
            label_max->setText(QString::number(max));
            label_total->setText(QString::number(total));
            label_current->setText(QString::number(current));
        }
        else if (list->at(2).contains(QString("----")))
        {
            for (int i = 3; i < list->size() - 1; i++)
            {
                msg = list->at(i);
                msg = msg.simplified();
                QStringList list_eu = msg.split(QString(" "));
                if (list_eu.size() >= 4)
                {
                    total += list_eu.at(list_eu.size() - 3).toULongLong();
                    max += list_eu.at(list_eu.size() - 2).toULongLong();
                    current += list_eu.at(list_eu.size() - 1).toULongLong();
                }
            }
            if (total == 0)
            {
                pro = 0;
            }
            else
            {
                pro = 100 - static_cast<int>((current * 100) / total);
            }
            if (progresswater->getValue() != pro)
            {
                progresswater->setValue(pro);
            }
            label_max->setText(QString::number(max));
            label_total->setText(QString::number(total));
            label_current->setText(QString::number(current));
        }
    }
}
void seimey_finsh::synchr_sem(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_sem\r\n");
    tree_synchr_sem = obj;
}
void seimey_finsh::ctl_synchr_sem(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_sem").length());
    msg = list->last();
    if (msg == head)
    {
        tree_synchr_sem->clear();
        for (int i = 3; i < list->size() - 1; i++)
        {
            msg = list->at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));
            if (list_eu.size() >= 3)
            {
                int index = 2;
                QTreeWidgetItem *child = tree_synchr_sem->currentItem();
                if(!child){
                    child = new QTreeWidgetItem();
                }
                for (int j = list_eu.size() - 1; j >= list_eu.size() - 3; j--)
                {
                    child->setText(index--, list_eu.at(j));
                }
                tree_synchr_sem->addTopLevelItem(child);
            }
        }
    }
}
void seimey_finsh::synchr_mutex(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_mutex\r\n");
    tree_synchr_mutex = obj;
}
void seimey_finsh::ctl_synchr_mutex(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_mutex").length());
    msg = list->last();
    if (msg == head)
    {
        tree_synchr_mutex->clear();
        for (int i = 3; i < list->size() - 1; i++)
        {
            msg = list->at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));
            if (list_eu.size() >= 4)
            {
                int index = 3;
                QTreeWidgetItem *child = tree_synchr_mutex->currentItem();
                if(!child){
                    child = new QTreeWidgetItem();
                }
                for (int j = list_eu.size() - 1; j >= list_eu.size() - 4; j--)
                {
                    child->setText(index--, list_eu.at(j));
                }
                tree_synchr_mutex->addTopLevelItem(child);
            }
        }
    }
}
void seimey_finsh::synchr_event(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_event\r\n");
    tree_synchr_event = obj;
}
void seimey_finsh::ctl_synchr_event(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_event").length());
    msg = list->last();
    if (msg == head)
    {
        tree_synchr_event->clear();
        for (int i = 3; i < list->size() - 1; i++)
        {
            msg = list->at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));
            if (list_eu.size() >= 3)
            {
                int index = 2;
                QTreeWidgetItem *child = tree_synchr_event->currentItem();
                if(!child){
                    child = new QTreeWidgetItem();
                }
                for (int j = list_eu.size() - 1; j >= list_eu.size() - 3; j--)
                {
                    child->setText(index--, list_eu.at(j));
                }
                tree_synchr_event->addTopLevelItem(child);
            }
        }
    }
}
void seimey_finsh::commun_mail(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_mailbox\r\n");
    tree_commun_mail = obj;
}
void seimey_finsh::ctl_commun_mail(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_mailbox").length());
    msg = list->last();
    if (msg == head)
    {
        tree_commun_mail->clear();
        for (int i = 3; i < list->size() - 1; i++)
        {
            msg = list->at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));
            if (list_eu.size() >= 4)
            {
                int index = 3;
                QTreeWidgetItem *child = tree_commun_mail->currentItem();
                if(!child){
                    child = new QTreeWidgetItem();
                }
                for (int j = list_eu.size() - 1; j >= list_eu.size() - 4; j--)
                {
                    child->setText(index--, list_eu.at(j));
                }
                tree_commun_mail->addTopLevelItem(child);
            }
        }
    }
}
void seimey_finsh::commun_queue(seimey_serial *Serial, QTreeWidget *obj)
{
    event |= 0x01;
    Serial->send_data("list_msgqueue\r\n");
    tree_commun_queue = obj;
}
void seimey_finsh::ctl_commun_queue(QStringList *list)
{
    QString head, msg;
    msg = list->at(0);
    head = msg.left(msg.length() - QString("list_msgqueue").length());
    msg = list->last();
    if (msg == head)
    {
        tree_commun_queue->clear();
        for (int i = 3; i < list->size() - 1; i++)
        {
            msg = list->at(i);
            msg = msg.simplified();
            QStringList list_eu = msg.split(QString(" "));
            if (list_eu.size() >= 3)
            {
                int index = 2;
                QTreeWidgetItem *child = tree_commun_queue->currentItem();
                if(!child){
                    child = new QTreeWidgetItem();
                }
                for (int j = list_eu.size() - 1; j >= list_eu.size() - 3; j--)
                {
                    child->setText(index--, list_eu.at(j));
                }
                tree_commun_queue->addTopLevelItem(child);
            }
        }
    }
}
void seimey_finsh::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
}
