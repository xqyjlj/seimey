#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include "seimey_data.h"
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    seimey_create_workspace();
    init_ctl();
    init_connect();
    qInfo() << "open";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_ctl(void)
{
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);

    setting.is_save_serial = seimey_get_is_save_serial_data();
    setting.timed_refresh = seimey_get_timed_refresh_time();

    ui->scrollA_task_communicate->setBackgroundRole(QPalette::Light); /* set the task_manager's communicate QScrollArea background (White) */
    ui->scrollA_task_synchronize->setBackgroundRole(QPalette::Light); /* set the task_manager's synchronize QScrollArea background (White) */
    ui->scrollA_task_property->setBackgroundRole(QPalette::Light); /* set the task_manager's property QScrollArea background (White) */

    serial_status.status = new QLabel("连接状态：", this);
    serial_status.baud_status = new QLabel("波特率：", this);
    serial_status.port_status = new QLabel("端口：", this);
    statusBar()->addPermanentWidget(serial_status.port_status);
    statusBar()->addPermanentWidget(serial_status.baud_status);
    statusBar()->addPermanentWidget(serial_status.status);

    finsh_timerID = new QTimer(this);
    connect(finsh_timerID, SIGNAL(timeout()), this, SLOT(finsh_timer_timeout()), Qt::UniqueConnection);
    if (setting.timed_refresh > 0)
    {
        finsh_timerID->setInterval(setting.timed_refresh * 1000);
        finsh_timerID->start();
    }
    QHeaderView *head = ui->treeW_file_mannage->header();
    head->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void MainWindow::init_connect(void)
{
    connect(seimey_serial_c, SIGNAL(already_recv_data()), this, SLOT(serial_data_coming()), Qt::UniqueConnection);
}

void MainWindow::on_menu_setting_serial_setting_triggered()
{
    seimey_qwchse_c = new seimey_qwchse(this);
    connect(seimey_qwchse_c, SIGNAL(window_close()), this, SLOT(on_menu_setting_serial_link_triggered()), Qt::UniqueConnection);
    seimey_qwchse_c->show();
}

void MainWindow::on_menu_setting_serial_link_triggered(void)
{
    bool status;
    status = seimey_serial_c->set_serial_link(ui->menu_setting_serial_link->isChecked());
    ui->menu_setting_serial_link->setChecked(status);
    QString tmp = status ? "已连接" : "未连接";
    serial_status.status->setText(QString("连接状态：") + tmp);
    serial_status.baud_status->setText(QString("波特率：") + seimey_serial_c->get_seial_baud());
    serial_status.port_status->setText(QString("端口：") + seimey_serial_c->get_seial_port());
}

void MainWindow::on_menu_help_about_triggered(void)
{
    QString dlgTitle = QStringLiteral("关于");
    QString strInfo = QStringLiteral("此开源软件是本人(小权一句两句)。\n"
                                     "在使用Windows的任务管理器时，有感而发。\n"
                                     "基于LGPL开源协议");
    QMessageBox::about(this, dlgTitle, strInfo);
}

void MainWindow::on_menu_help_github_triggered(void)
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/xqyjlj/seimey")));
}

void MainWindow::on_menu_setting_preference_triggered()
{
    seimey_setting_c = new seimey_setting(this);
    connect(seimey_setting_c, &seimey_setting::window_close, [ = ]()
    {
        setting.is_save_serial = seimey_get_is_save_serial_data();
        setting.timed_refresh = seimey_get_timed_refresh_time();
        if (setting.timed_refresh > 0)
        {
            finsh_timerID->setInterval(setting.timed_refresh * 1000);
            finsh_timerID->start();
        }
        else
        {
            finsh_timerID->stop();
        }
    });
    seimey_setting_c->show();
}

void MainWindow::on_menu_setting_serial_setting_2_triggered()
{
    on_menu_setting_serial_setting_triggered();
}

void MainWindow::assign_finsh_mode(int mode, QStringList *list)
{
    switch (mode)
    {
    case RT_THREAD:
        finsh_thread(list);
        break;
    case RT_DEVICE:
        finsh_device(list);
        break;
    case RT_TIMER:
        finsh_timer(list);
        break;
    case RT_MEMTOOL:
        finsh_mempool(list);
        break;
    case RT_MEMHEAP:
        finsh_memheap(list);
        break;
    case RT_FREE:
        finsh_memfree(list);
        break;
    case RT_SEM:
        finsh_sem(list);
        break;
    case RT_MUTEX:
        finsh_mutex(list);
        break;
    case RT_EVENT:
        finsh_event(list);
        break;
    case RT_MAILBOX:
        finsh_mailbox(list);
        break;
    case RT_MSGQUEUE:
        finsh_msgqueue(list);
        break;
    }
}

static void save_serial_data(QString string)
{
    static QString qs_serial_data_file = QDir::currentPath() + "/.workspace" + "/.serial" + "/serial.txt";
    QString time = QDateTime::currentDateTime().toString(QString("[ yyyy-MM-dd HH:mm:ss:zzz ]"));
    QString mmsg;
    mmsg = QString("%1 %2\r\n").arg(time).arg(string);
    QFile file(qs_serial_data_file);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << mmsg;
    file.flush();
    file.close();
}

void MainWindow::serial_data_coming(void)
{
    QString tmp;
    static QString head;
    static enum_finsh_status finsh_status = FINSH_NULL;
    static QStringList serial_rx_list;
    static int index = 0;
    tmp = serial_rx_queue.dequeue();
    if (setting.is_save_serial)
    {
        save_serial_data(tmp);
    }
    if (finsh_status == FINSH_NULL)
    {
        head.clear();
        if (tmp.length() > 6)
        {
            if (tmp.contains("msh ") && tmp.count(">") == 1)
            {
                head = tmp.left(tmp.indexOf(">") + 1);
                finsh_status = FINSH_GET_MSH;
            }
            else finsh_status = FINSH_NULL;
        }
        else finsh_status = FINSH_NULL;

        if (finsh_status == FINSH_GET_MSH)
        {
            tmp.remove(0, head.length());
            index = finsh_mode_list.indexOf(tmp);
            if (index >= 0)
            {
                finsh_status = FINSH_GET_COMMAND;
            }
            else finsh_status = FINSH_NULL;
        }
        else finsh_status = FINSH_NULL;
    }
    else if (finsh_status == FINSH_GET_COMMAND)
    {

        if (tmp == head)
        {
            finsh_status = FINSH_DOWN;
            head.clear();
        }
        else
        {
            serial_rx_list << tmp;
        }
    }
    else finsh_status = FINSH_NULL;

    if (finsh_status == FINSH_DOWN)
    {
        finsh_status = FINSH_NULL;
        assign_finsh_mode(index, &serial_rx_list);
        serial_rx_list.clear();
    }
}

static QTreeWidgetItem *finsh_child(int count, QString string)
{
    string = string.simplified();
    QStringList list = string.split(" ");
    int size = list.size();
    QString tmp;
    if (size >= count)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setFlags(child->flags() | Qt::ItemIsEditable);

        for (int j = size - 1; j >= size - count; j--)
        {
            child->setText(j - size + count, list.at(j));
        }
        if (size > count)
        {
            for (int i = 0; i < size - count + 1; i++)
            {
                tmp = tmp + list.at(i) + " ";
            }

            child->setText(0, tmp);
        }
        return child;
    }
    else
    {
        return NULL;
    }

}

void MainWindow::finsh_thread(QStringList *list)
{
    ui->treeW_finsh_thread->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(8, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_thread->addTopLevelItem(child);
        }
    }
}

void MainWindow::finsh_device(QStringList *list)
{
    ui->treeW_finsh_device->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setFlags(child->flags() | Qt::ItemIsEditable);
        QString tmp = list->at(i);
        tmp = tmp.simplified();
        QStringList tmp_list = tmp.split(" ");
        child->setText(0, tmp_list.at(0));
        QString type;
        if (tmp_list.size() >= 3)
        {
            for (int j = 1; j < tmp_list.size() - 1; j++)
            {
                type += tmp_list.at(j);
                type += " ";
            }
            child->setText(1, type);
            child->setText(2, tmp_list.last());
            ui->treeW_finsh_device->addTopLevelItem(child);
        }
    }
}

void MainWindow::finsh_timer(QStringList *list)
{
    ui->treeW_finsh_timer->clear();
    for (int i = 2; i < list->size() - 1; ++i)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setFlags(child->flags() | Qt::ItemIsEditable);
        QString tmp = list->at(i);
        tmp = tmp.simplified();
        QStringList tmp_list = tmp.split(" ");
        if (tmp_list.size() >= 4)
        {
            for (int j = 0; j < 4; j++)
            {
                child->setText(j, tmp_list.at(j));
            }
            ui->treeW_finsh_timer->addTopLevelItem(child);
        }
    }
    QString tmp = list->last();
    tmp.remove(0, 13);
    ui->lineE_current_tick->setText(tmp);
}

void MainWindow::finsh_mempool(QStringList *list)
{
    ui->treeW_finsh_mempool->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(5, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_mempool->addTopLevelItem(child);
        }
    }
}

void MainWindow::finsh_memheap(QStringList *list)
{
    ui->treeW_finsh_memheap->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(4, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_memheap->addTopLevelItem(child);
        }
    }
}

void MainWindow::finsh_memfree(QStringList *list)
{
    QString tmp;
    QStringList tmp_list;
    qulonglong total_memory = 0,
               used_memory = 0,
               max_used_memory = 0,
               remaining_available_memory = 0;
    int progress;
    if (list->size() >= 3)
    {
        if (list->at(1).contains("used memory :"))
        {
            tmp = list->at(0);
            tmp = tmp.simplified();
            tmp_list = tmp.split(" ");
            total_memory = tmp_list.last().toULongLong();

            tmp = list->at(1);
            tmp = tmp.simplified();
            tmp_list = tmp.split(" ");
            used_memory = tmp_list.last().toULongLong();

            tmp = list->at(2);
            tmp = tmp.simplified();
            tmp_list = tmp.split(" ");
            max_used_memory = tmp_list.last().toULongLong();

            remaining_available_memory = total_memory - used_memory;
        }
        else
        {
            for (int i = 2; i < list->size(); ++i)
            {
                tmp = list->at(i);
                tmp = tmp.simplified();
                tmp_list = tmp.split(" ");
                total_memory += tmp_list.at(1).toInt();
                max_used_memory += tmp_list.at(2).toInt();
                remaining_available_memory += tmp_list.at(3).toInt();
            }
        }

        progress = ((total_memory - remaining_available_memory) * 100 / total_memory);
        ui->proW_memfree->setValue(progress);
        ui->label_total_mem->setText(QString::number(total_memory));
        ui->label_maxused_mem->setText(QString::number(max_used_memory));
        ui->label_remaining_mem->setText(QString::number(remaining_available_memory));
    }
}

void MainWindow::finsh_sem(QStringList *list)
{
    ui->treeW_finsh_sem->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(3, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_sem->addTopLevelItem(child);
        }
    }
}

void MainWindow::finsh_mutex(QStringList *list)
{
    ui->treeW_finsh_mutex->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(4, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_mutex->addTopLevelItem(child);
        }
    }
}

void MainWindow::finsh_event(QStringList *list)
{
    ui->treeW_finsh_event->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(3, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_event->addTopLevelItem(child);
        }
    }
}

void MainWindow::finsh_mailbox(QStringList *list)
{
    ui->treeW_finsh_mailbox->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(4, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_event->addTopLevelItem(child);
        }
    }
}

void MainWindow::finsh_msgqueue(QStringList *list)
{
    ui->treeW_finsh_msqueue->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(3, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_msqueue->addTopLevelItem(child);
        }
    }
}

void MainWindow::on_tabW_task_manager_tabBarDoubleClicked(int index)
{
    if (seimey_serial_c->get_serial_status())
    {
        switch (index)
        {
        case 0:
            seimey_serial_c->serial_send_data("list_thread\r\n");
            break;
        case 1:
            if (ui->comdLB_mem_pool->isChecked())
                assign_button_mem(MEM_POOL);
            else if (ui->comdLB_mem_heap->isChecked())
                assign_button_mem(MEM_HEAP);
            else if (ui->comdLB_mem_free->isChecked())
                assign_button_mem(MEM_FREE);
            break;
        case 2:
            if (ui->comdLB_synchronize_event->isChecked())
                assign_button_synchronize(SYNCHR_EVENT);
            else if (ui->comdLB_synchronize_mutex->isChecked())
                assign_button_synchronize(SYNCHR_MUTEX);
            else if (ui->comdLB_synchronize_semaphore->isChecked())
                assign_button_synchronize(SYNCHR_SEM);
            break;
        case 3:
            if (ui->comdLB_communicate_mail->isChecked())
                assign_button_communicate(COMMUN_MAIL);
            else if (ui->comdLB_communicate_mail->isChecked())
                assign_button_communicate(COMMUN_QUEUE);
            break;
        case 4:
            seimey_serial_c->serial_send_data("list_device\r\n");
            break;
        case 5:
            seimey_serial_c->serial_send_data("list_timer\r\n");
            break;
        }
    }
    else QMessageBox::warning(NULL, "警告", "串口写入错误(可能是没有打开串口)");
}

void MainWindow::on_tabW_task_manager_currentChanged(int index)
{
    if (seimey_serial_c->get_serial_status())
    {
        switch (index)
        {
        case 0:
            seimey_serial_c->serial_send_data("list_thread\r\n");
            break;
        case 1:
            if (ui->comdLB_mem_pool->isChecked())
                assign_button_mem(MEM_POOL);
            else if (ui->comdLB_mem_heap->isChecked())
                assign_button_mem(MEM_HEAP);
            else if (ui->comdLB_mem_free->isChecked())
                assign_button_mem(MEM_FREE);
            break;
        case 2:
            if (ui->comdLB_synchronize_event->isChecked())
                assign_button_synchronize(SYNCHR_EVENT);
            else if (ui->comdLB_synchronize_mutex->isChecked())
                assign_button_synchronize(SYNCHR_MUTEX);
            else if (ui->comdLB_synchronize_semaphore->isChecked())
                assign_button_synchronize(SYNCHR_SEM);
            break;
        case 3:
            if (ui->comdLB_communicate_mail->isChecked())
                assign_button_communicate(COMMUN_MAIL);
            else if (ui->comdLB_communicate_mail->isChecked())
                assign_button_communicate(COMMUN_QUEUE);
            break;
        case 4:
            seimey_serial_c->serial_send_data("list_device\r\n");
            break;
        case 5:
            seimey_serial_c->serial_send_data("list_timer\r\n");
            break;
        }
    }
}

void MainWindow::on_comdLB_mem_pool_clicked(void)
{
    assign_button_mem(MEM_POOL);
}

void MainWindow::on_comdLB_mem_heap_clicked(void)
{
    assign_button_mem(MEM_HEAP);
}

void MainWindow::on_comdLB_mem_free_clicked(void)
{
    assign_button_mem(MEM_FREE);
}

void MainWindow::assign_button_mem(int mode)
{
    switch (mode)
    {
    case MEM_POOL:
        ui->comdLB_mem_pool->setChecked(true);
        ui->comdLB_mem_free->setChecked(false);
        ui->comdLB_mem_heap->setChecked(false);
        ui->stackedW_property->setCurrentIndex(0);
        seimey_serial_c->serial_send_data("list_mempool\r\n");
        break;
    case MEM_HEAP:
        ui->comdLB_mem_heap->setChecked(true);
        ui->comdLB_mem_free->setChecked(false);
        ui->comdLB_mem_pool->setChecked(false);
        ui->stackedW_property->setCurrentIndex(1);
        seimey_serial_c->serial_send_data("list_memheap\r\n");
        break;
    case MEM_FREE:
        ui->comdLB_mem_free->setChecked(true);
        ui->comdLB_mem_pool->setChecked(false);
        ui->comdLB_mem_heap->setChecked(false);
        ui->stackedW_property->setCurrentIndex(2);
        seimey_serial_c->serial_send_data("free\r\n");
        break;
    }
}

void MainWindow::on_comdLB_synchronize_semaphore_clicked()
{
    assign_button_synchronize(SYNCHR_SEM);
}

void MainWindow::on_comdLB_synchronize_mutex_clicked()
{
    assign_button_synchronize(SYNCHR_MUTEX);
}

void MainWindow::on_comdLB_synchronize_event_clicked()
{
    assign_button_synchronize(SYNCHR_EVENT);
}

void MainWindow::assign_button_synchronize(int mode)
{
    switch (mode)
    {
    case SYNCHR_SEM:
        ui->comdLB_synchronize_semaphore->setChecked(true);
        ui->comdLB_synchronize_mutex->setChecked(false);
        ui->comdLB_synchronize_event->setChecked(false);
        ui->stackedW_synchronize->setCurrentIndex(0);
        seimey_serial_c->serial_send_data("list_sem\r\n");
        break;
    case SYNCHR_MUTEX:
        ui->comdLB_synchronize_mutex->setChecked(true);
        ui->comdLB_synchronize_semaphore->setChecked(false);
        ui->comdLB_synchronize_event->setChecked(false);
        ui->stackedW_synchronize->setCurrentIndex(1);
        seimey_serial_c->serial_send_data("list_mutex\r\n");
        break;
    case SYNCHR_EVENT:
        ui->comdLB_synchronize_event->setChecked(true);
        ui->comdLB_synchronize_semaphore->setChecked(false);
        ui->comdLB_synchronize_mutex->setChecked(false);
        ui->stackedW_synchronize->setCurrentIndex(2);
        seimey_serial_c->serial_send_data("list_event\r\n");
        break;
    }
}

void MainWindow::on_comdLB_communicate_mail_clicked()
{
    assign_button_communicate(COMMUN_MAIL);
}

void MainWindow::on_comdLB_communicate_queue_clicked()
{
    assign_button_communicate(COMMUN_QUEUE);
}

void MainWindow::assign_button_communicate(int mode)
{
    switch (mode)
    {
    case COMMUN_MAIL:
        ui->comdLB_communicate_mail->setChecked(true);
        ui->comdLB_communicate_queue->setChecked(false);
        ui->stackedW_communicate->setCurrentIndex(1);
        seimey_serial_c->serial_send_data("list_mailbox\r\n");
        break;
    case COMMUN_QUEUE:
        ui->comdLB_communicate_queue->setChecked(true);
        ui->comdLB_communicate_mail->setChecked(false);
        ui->stackedW_communicate->setCurrentIndex(0);
        \
        seimey_serial_c->serial_send_data("list_msgqueue\r\n");
        break;
    }
}

void MainWindow::on_menu_setting_preference_2_triggered()
{
    on_menu_setting_preference_triggered();
}

void MainWindow::finsh_timer_timeout()
{
    on_tabW_task_manager_currentChanged(ui->tabW_task_manager->currentIndex());
}
