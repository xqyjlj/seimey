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
    seimey_Create_Dir();
    init_ctl();
    init_connect();
    setAttribute(Qt::WA_QuitOnClose, true);
    qInfo() << "open";
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
 *  初始化界面的控件
*/
void MainWindow::init_ctl(void)
{
    /* 设置主界面背景为白色 */
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    /* 获取设置 */
    setting.is_save_serial = seimey_Get_Is_Save_Serial_Data();
    setting.timed_refresh = seimey_Get_Timed_Refresh_Time();
    /* 设置QScrollArea为Light风格 */
    ui->scrollA_task_communicate->setBackgroundRole(QPalette::Light);
    ui->scrollA_task_synchronize->setBackgroundRole(QPalette::Light);
    ui->scrollA_task_property->setBackgroundRole(QPalette::Light);
    /* 设置 */
    serial_status.status = new QLabel("连接状态：", this);
    serial_status.baud_status = new QLabel("波特率：", this);
    serial_status.port_status = new QLabel("端口：", this);
    statusBar()->addPermanentWidget(serial_status.port_status);
    statusBar()->addPermanentWidget(serial_status.baud_status);
    statusBar()->addPermanentWidget(serial_status.status);
    /* 创建定时器 */
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
/*
 *  用来初始化一些connect函数
*/
void MainWindow::init_connect(void)
{
    connect(c_Seimey_Serial, SIGNAL(already_recv_data()), this, SLOT(serial_data_coming()), Qt::UniqueConnection);
}
/*
 * 菜单·串口设置按键点击槽函数
*/
void MainWindow::on_menu_setting_serial_setting_triggered()
{
    c_Seimey_Qwchse = new seimey_qwchse(this);
    connect(c_Seimey_Qwchse, &seimey_qwchse::window_close, [ = ]()
    {
        on_menu_setting_serial_link_triggered();
        c_Seimey_Qwchse = NULL;
    });
    c_Seimey_Qwchse->show();
}
/*
 * 菜单·串口连接按键点击槽函数
*/
void MainWindow::on_menu_setting_serial_link_triggered(void)
{
    bool status;
    status = c_Seimey_Serial->set_serial_link(ui->menu_setting_serial_link->isChecked());
    ui->menu_setting_serial_link->setChecked(status);
    QString tmp = status ? "已连接" : "未连接";
    serial_status.status->setText(QString("连接状态：") + tmp);
    serial_status.baud_status->setText(QString("波特率：") + c_Seimey_Serial->get_seial_baud());
    serial_status.port_status->setText(QString("端口：") + c_Seimey_Serial->get_seial_port());
}
/*
 * 菜单·帮助·关于按键点击槽函数
*/
void MainWindow::on_menu_help_about_triggered(void)
{
    QString dlgTitle = QStringLiteral("关于");
    QString strInfo = QStringLiteral("此开源软件是本人(小权一句两句)。\n"
                                     "在使用Windows的任务管理器时，有感而发。\n"
                                     "此软件完全免费，但是还是可以联系作者给作者打赏一杯奶茶钱。\n"
                                     "基于LGPL开源协议");
    QMessageBox::about(this, dlgTitle, strInfo);
}
/*
 * 菜单·帮助·github地址按键点击槽函数
*/
void MainWindow::on_menu_help_github_triggered(void)
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/xqyjlj/seimey")));
}
/*
 * 菜单·首选项设置按键点击槽函数
*/
void MainWindow::on_menu_setting_preference_triggered()
{
    c_Seimey_Setting = new seimey_setting(this);
    connect(c_Seimey_Setting, &seimey_setting::window_close, [ = ]()
    {
        /* 获取设置的变量 */
        setting.is_save_serial = seimey_Get_Is_Save_Serial_Data();
        setting.timed_refresh = seimey_Get_Timed_Refresh_Time();
        /* 如果启动了定时刷新，则会启动定时器 */
        if (setting.timed_refresh > 0)
        {
            finsh_timerID->setInterval(setting.timed_refresh * 1000);
            finsh_timerID->start();
        }
        else
        {
            finsh_timerID->stop();
        }
        c_Seimey_Setting = NULL;
    });
    c_Seimey_Setting->show();
}
/*
 * 菜单·串口设置按键点击槽函数
*/
void MainWindow::on_menu_setting_serial_setting_2_triggered()
{
    on_menu_setting_serial_setting_triggered();
}
/*
 * finsh基本命令的控制分流函数
*/
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
/*
 * 任务管理器的控制函数
*/
void MainWindow::task_handle(QString msg)
{
    static QString head;
    static enum_msh_status msh_status = MSH_NULL;
    static QStringList serial_rx_list;
    static int index = 0;
    if (msh_status == MSH_NULL)
    {
        head.clear();/* 首先清除头部信息 */
        /* 命令最小长度“msh >”即为长度为5 */
        if (msg.length() >= 5)
        {
            /* 如果包含了msh，以及语句中只有一个'>' */
            if (msg.contains("msh ") && msg.count(">") == 1)
            {
                head = msg.left(msg.indexOf(">") + 1);/* 得到头部信息 */
                msh_status = MSH_GET_MSH;
            }
            else msh_status = MSH_NULL;
        }
        else msh_status = MSH_NULL;
        /* 如果得到了MSH关键字 */
        if (msh_status == MSH_GET_MSH)
        {
            msg.remove(0, head.length());
            index = finsh_mode_list.indexOf(msg);/* 获取命令在命令表的节点 */
            if (index > 0)
            {
                msh_status = MSH_GET_COMMAND;
            }
            else msh_status = MSH_NULL;
        }
        else msh_status = MSH_NULL;
    }

    if (msh_status == MSH_GET_COMMAND)
    {

        if (msg == head)
        {
            msh_status = MSH_DOWN;
            head.clear();
        }
        else
        {
            serial_rx_list << msg;/* 将串口信息加入列表 */
        }
    }

    if (msh_status == MSH_DOWN)
    {
        msh_status = MSH_NULL;
        assign_finsh_mode(index, &serial_rx_list);/* 将数据载入分配器 */
        serial_rx_list.clear();/* 清空列表 */
    }
}
/*
 * 文件管理器的控制函数
*/
void MainWindow::dfs_handle(QString data)
{
    static QString head;
    static enum_msh_status msh_status = MSH_NULL;
    static int index = 0;
    qDebug("%x\r\n", data.toLatin1().data()[0]);
    if (msh_status == MSH_NULL)
    {
        head.clear();
        if (data.length() >= 5)
        {
            if (data.contains("msh ") && data.count(">") == 1)
            {
                head = data.left(data.indexOf(">") + 1);
                msh_status = MSH_GET_MSH;
            }
            else msh_status = MSH_NULL;
        }
        else msh_status = MSH_NULL;

        if (msh_status == MSH_GET_MSH)
        {
            data.remove(0, head.length());
            index = dfs_mode_list.indexOf(data);
            if (data >= 0)
            {
                msh_status = MSH_GET_COMMAND;
            }
            else msh_status = MSH_NULL;
        }
        else msh_status = MSH_NULL;
    }
}
/*
 * 串口数据来临槽函数
*/
void MainWindow::serial_data_coming(void)
{
    QString tmp;
    tmp = serial_rx_queue.dequeue();
    if (setting.is_save_serial)
    {
        seimey_Save_Serial_Data(tmp);
    }
    switch (ui->tabW_main->currentIndex())
    {
    case 0:
        task_handle(tmp);
        break;
    case 1:
        dfs_handle(tmp);
        break;
    default:
        break;
    }

}
/*
 * 创建一个任务管理器的QTreeWidgetItem对象
*/
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
/*
 * finsh Thread
*/
void MainWindow::finsh_thread(QStringList *list)
{
    ui->treeW_finsh_thread->clear();
    for (int i = 3; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(8, tmp);
        if (child != NULL)
        {
            QString icon = ":/icon/qrc/icon/thread_obj_" + QString::number((i - 2) % 3) + ".png";
            child->setIcon(0, QIcon(icon));
            ui->treeW_finsh_thread->addTopLevelItem(child);
        }
    }
}
/*
 * finsh Device
*/
void MainWindow::finsh_device(QStringList *list)
{
    ui->treeW_finsh_device->clear();
    for (int i = 3; i < list->size(); ++i)
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
            QString icon = ":/icon/qrc/icon/device_" + QString::number((i - 2) % 3) + ".png";
            child->setIcon(0, QIcon(icon));
            ui->treeW_finsh_device->addTopLevelItem(child);
        }
    }
}
/*
 * finsh Timer
*/
void MainWindow::finsh_timer(QStringList *list)
{
    ui->treeW_finsh_timer->clear();
    for (int i = 3; i < list->size() - 1; ++i)
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
            QString icon = ":/icon/qrc/icon/timer_" + QString::number((i - 2) % 3) + ".png";
            child->setIcon(0, QIcon(icon));
            ui->treeW_finsh_timer->addTopLevelItem(child);
        }
    }
    QString tmp = list->last();
    tmp.remove(0, 13);
    ui->lineE_current_tick->setText(tmp);
}
/*
 * finsh Mempool
*/
void MainWindow::finsh_mempool(QStringList *list)
{
    ui->treeW_finsh_mempool->clear();
    for (int i = 3; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(5, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_mempool->addTopLevelItem(child);
        }
    }
}
/*
 * finsh Memheap
*/
void MainWindow::finsh_memheap(QStringList *list)
{
    ui->treeW_finsh_memheap->clear();
    for (int i = 3; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(4, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_memheap->addTopLevelItem(child);
        }
    }
}
/*
 * finsh Memfree
*/
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
            for (int i = 3; i < list->size(); ++i)
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
/*
 * finsh Sem
*/
void MainWindow::finsh_sem(QStringList *list)
{
    ui->treeW_finsh_sem->clear();
    for (int i = 3; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(3, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_sem->addTopLevelItem(child);
        }
    }
}
/*
 * finsh Mutex
*/
void MainWindow::finsh_mutex(QStringList *list)
{
    ui->treeW_finsh_mutex->clear();
    for (int i = 3; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(4, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_mutex->addTopLevelItem(child);
        }
    }
}
/*
 * finsh Event
*/
void MainWindow::finsh_event(QStringList *list)
{
    ui->treeW_finsh_event->clear();
    for (int i = 3; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(3, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_event->addTopLevelItem(child);
        }
    }
}
/*
 * finsh Mailbox
*/
void MainWindow::finsh_mailbox(QStringList *list)
{
    ui->treeW_finsh_mailbox->clear();
    for (int i = 3; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(4, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_event->addTopLevelItem(child);
        }
    }
}
/*
 * finsh Msgqueue
*/
void MainWindow::finsh_msgqueue(QStringList *list)
{
    ui->treeW_finsh_msqueue->clear();
    for (int i = 3; i < list->size(); ++i)
    {
        QString tmp = list->at(i);
        QTreeWidgetItem *child = finsh_child(3, tmp);
        if (child != NULL)
        {
            ui->treeW_finsh_msqueue->addTopLevelItem(child);
        }
    }
}
/*
 * 任务管理器顶端标签双击槽函数
*/
void MainWindow::on_tabW_task_manager_tabBarDoubleClicked(int index)
{
    if (c_Seimey_Serial->get_serial_status())
    {
        switch (index)
        {
        case 0:
            c_Seimey_Serial->serial_send_data("list_thread\r\n");
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
            c_Seimey_Serial->serial_send_data("list_device\r\n");
            break;
        case 5:
            c_Seimey_Serial->serial_send_data("list_timer\r\n");
            break;
        }
    }
    else QMessageBox::warning(NULL, "警告", "串口写入错误(可能是没有打开串口)");
}
/*
 * 任务管理器顶端标签改变槽函数
*/
void MainWindow::on_tabW_task_manager_currentChanged(int index)
{
    if (c_Seimey_Serial->get_serial_status())
    {
        switch (index)
        {
        case 0:
            c_Seimey_Serial->serial_send_data("list_thread\r\n");
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
            c_Seimey_Serial->serial_send_data("list_device\r\n");
            break;
        case 5:
            c_Seimey_Serial->serial_send_data("list_timer\r\n");
            break;
        }
    }
}
/*
 * 内存池按钮点击槽函数
*/
void MainWindow::on_comdLB_mem_pool_clicked(void)
{
    assign_button_mem(MEM_POOL);
}
/*
 * 内存堆按钮点击槽函数
*/
void MainWindow::on_comdLB_mem_heap_clicked(void)
{
    assign_button_mem(MEM_HEAP);
}
/*
 * 动态按钮点击槽函数
*/
void MainWindow::on_comdLB_mem_free_clicked(void)
{
    assign_button_mem(MEM_FREE);
}
/*
 * finsh内存命令的控制分流函数
*/
void MainWindow::assign_button_mem(int mode)
{
    switch (mode)
    {
    case MEM_POOL:
        ui->comdLB_mem_pool->setChecked(true);
        ui->comdLB_mem_free->setChecked(false);
        ui->comdLB_mem_heap->setChecked(false);
        ui->stackedW_property->setCurrentIndex(0);
        c_Seimey_Serial->serial_send_data("list_mempool\r\n");
        break;
    case MEM_HEAP:
        ui->comdLB_mem_heap->setChecked(true);
        ui->comdLB_mem_free->setChecked(false);
        ui->comdLB_mem_pool->setChecked(false);
        ui->stackedW_property->setCurrentIndex(1);
        c_Seimey_Serial->serial_send_data("list_memheap\r\n");
        break;
    case MEM_FREE:
        ui->comdLB_mem_free->setChecked(true);
        ui->comdLB_mem_pool->setChecked(false);
        ui->comdLB_mem_heap->setChecked(false);
        ui->stackedW_property->setCurrentIndex(2);
        c_Seimey_Serial->serial_send_data("free\r\n");
        break;
    }
}
/*
 * 信号量按钮点击槽函数
*/
void MainWindow::on_comdLB_synchronize_semaphore_clicked()
{
    assign_button_synchronize(SYNCHR_SEM);
}
/*
 * 二值信号量按钮点击槽函数
*/
void MainWindow::on_comdLB_synchronize_mutex_clicked()
{
    assign_button_synchronize(SYNCHR_MUTEX);
}
/*
 * 事件按钮点击槽函数
*/
void MainWindow::on_comdLB_synchronize_event_clicked()
{
    assign_button_synchronize(SYNCHR_EVENT);
}
/*
 * finsh同步命令的控制分流函数
*/
void MainWindow::assign_button_synchronize(int mode)
{
    switch (mode)
    {
    case SYNCHR_SEM:
        ui->comdLB_synchronize_semaphore->setChecked(true);
        ui->comdLB_synchronize_mutex->setChecked(false);
        ui->comdLB_synchronize_event->setChecked(false);
        ui->stackedW_synchronize->setCurrentIndex(0);
        c_Seimey_Serial->serial_send_data("list_sem\r\n");
        break;
    case SYNCHR_MUTEX:
        ui->comdLB_synchronize_mutex->setChecked(true);
        ui->comdLB_synchronize_semaphore->setChecked(false);
        ui->comdLB_synchronize_event->setChecked(false);
        ui->stackedW_synchronize->setCurrentIndex(1);
        c_Seimey_Serial->serial_send_data("list_mutex\r\n");
        break;
    case SYNCHR_EVENT:
        ui->comdLB_synchronize_event->setChecked(true);
        ui->comdLB_synchronize_semaphore->setChecked(false);
        ui->comdLB_synchronize_mutex->setChecked(false);
        ui->stackedW_synchronize->setCurrentIndex(2);
        c_Seimey_Serial->serial_send_data("list_event\r\n");
        break;
    }
}
/*
 * 邮箱按钮点击槽函数
*/
void MainWindow::on_comdLB_communicate_mail_clicked()
{
    assign_button_communicate(COMMUN_MAIL);
}
/*
 * 队列按钮点击槽函数
*/
void MainWindow::on_comdLB_communicate_queue_clicked()
{
    assign_button_communicate(COMMUN_QUEUE);
}
/*
 * finsh通信命令的控制分流函数
*/
void MainWindow::assign_button_communicate(int mode)
{
    switch (mode)
    {
    case COMMUN_MAIL:
        ui->comdLB_communicate_mail->setChecked(true);
        ui->comdLB_communicate_queue->setChecked(false);
        ui->stackedW_communicate->setCurrentIndex(1);
        c_Seimey_Serial->serial_send_data("list_mailbox\r\n");
        break;
    case COMMUN_QUEUE:
        ui->comdLB_communicate_queue->setChecked(true);
        ui->comdLB_communicate_mail->setChecked(false);
        ui->stackedW_communicate->setCurrentIndex(0);
        c_Seimey_Serial->serial_send_data("list_msgqueue\r\n");
        break;
    }
}
/*
 * 菜单·首选项设置按键点击槽函数
*/
void MainWindow::on_menu_setting_preference_2_triggered()
{
    on_menu_setting_preference_triggered();
}
/*
 * finsh定时刷新触发的槽函数
*/
void MainWindow::finsh_timer_timeout()
{
    on_tabW_task_manager_currentChanged(ui->tabW_task_manager->currentIndex());
}
/*
 * 菜单·插件按键点击槽函数
*/
void MainWindow::on_menu_plugin_triggered()
{
    if (plugin.seimey_plugin_c == NULL)
    {
        plugin.seimey_plugin_c = new seimey_plugin(this);
        plugin.isexit = true;
        plugin.seimey_plugin_c->show();
        connect(plugin.seimey_plugin_c, &seimey_plugin::window_Close, [ = ]()
        {
            plugin.isexit = false;
            plugin.seimey_plugin_c = NULL;
            qDebug() << "close the seimey_plugin";
        });
    }
}
/*
 * 菜单·插件按键点击槽函数
*/
void MainWindow::on_menu_plugin_2_triggered()
{
    on_menu_plugin_triggered();
}
