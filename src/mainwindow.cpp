#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>

struct
{
    QLabel *port_status;
    QLabel *baud_status;
    QLabel *status;
} serial_status;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    create_workspace();
    init_ctl();
    init_connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_ctl(void)
{
    ui->scrollA_task_communicate->setBackgroundRole(QPalette::Light); /* set the task_manager's communicate QScrollArea background (White) */
    ui->scrollA_task_synchronize->setBackgroundRole(QPalette::Light); /* set the task_manager's synchronize QScrollArea background (White) */
    ui->scrollA_task_property->setBackgroundRole(QPalette::Light); /* set the task_manager's property QScrollArea background (White) */

    serial_status.status = new QLabel("连接状态：", this);
    serial_status.baud_status = new QLabel("波特率：", this);
    serial_status.port_status = new QLabel("端口：", this);
    statusBar()->addPermanentWidget(serial_status.port_status);
    statusBar()->addPermanentWidget(serial_status.baud_status);
    statusBar()->addPermanentWidget(serial_status.status);
}

void MainWindow::init_connect(void)
{
    connect(seimey_serial_c, SIGNAL(already_recv_data()), this, SLOT(serial_data_coming()));
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

void MainWindow::create_workspace(void)
{
    /* create workspace folder */
    QString qs_workspace_dir = QDir::currentPath() + "/.workspace";
    QDir workspace_dir(qs_workspace_dir);
    if (!workspace_dir.exists())
    {
        workspace_dir.mkdir(qs_workspace_dir);
    }

    /* create serial folder */
    QString qs_serial_dir = qs_workspace_dir + "/.serial";
    QDir serial_dir(qs_serial_dir);
    if (!serial_dir.exists())
    {
        serial_dir.mkdir(qs_serial_dir);
    }

    QString qs_serial_file = qs_serial_dir + "/serial.json";
    QFile serial_file(qs_serial_file);
    if (!serial_file.exists())
    {
        serial_file.open(QIODevice::Append | QIODevice::Text);
        unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        serial_file.write((char *)bom, sizeof(bom));
        QTextStream in(&serial_file);
        in.setCodec("UTF-8");
        in << "{\n";
        in << "    \"SerialPort\": \"\",\n";
        in << "    \"BaudRate\": \"\",\n";
        in << "    \"DataBits\": \"\",\n";
        in << "    \"FlowControl\": \"\",\n";
        in << "    \"Parity\": \"\",\n";
        in << "    \"StopBits\": \"\"\n";
        in << "}";
        serial_file.close();
    }
}

void MainWindow::on_menu_setting_preference_triggered()
{

}

void MainWindow::on_menu_setting_serial_setting_2_triggered()
{
    on_menu_setting_serial_setting_triggered();
}

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

enum enum_finsh_status
{
    FINSH_NULL = 0,
    FINSH_GET_MSH,
    FINSH_GET_COMMAND,
    FINSH_DOWN
};
static QStringList finsh_mode_list = {"",
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

void MainWindow::assign_finsh_mode(int mode, QStringList *list)
{
    if (mode == RT_THREAD)
        finsh_thread(list);
    else if (mode == RT_DEVICE)
        finsh_device(list);
    else if (mode == RT_TIMER)
        finsh_timer(list);
}

void MainWindow::serial_data_coming(void)
{
    QString tmp;
    static enum_finsh_status finsh_status = FINSH_NULL;
    static QStringList serial_rx_list;
    static int index = 0;
    tmp = serial_rx_queue.dequeue();
    qDebug() << tmp;
    if (finsh_status == FINSH_NULL)
    {
        if (tmp.contains("msh />"))
        {
            finsh_status = FINSH_GET_MSH;
        }
        else finsh_status = FINSH_NULL;

        if (finsh_status == FINSH_GET_MSH)
        {
            tmp.remove(0, 6);
            qDebug() << "             " << tmp;
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

        if (tmp == "msh />")
        {
            finsh_status = FINSH_DOWN;
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

void MainWindow::finsh_thread(QStringList *list)
{
    ui->treeW_finsh_thread->clear();
    for (int i = 2; i < list->size(); ++i)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setFlags(child->flags() | Qt::ItemIsEditable);
        QString tmp = list->at(i);
        tmp = tmp.simplified();
        QStringList tmp_list = tmp.split(" ");
        for (int j = 0; j < 8; j++)
        {
            child->setText(j, tmp_list.at(j));
        }
        ui->treeW_finsh_thread->addTopLevelItem(child);
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
        for (int j = 0; j < 4; j++)
        {
            child->setText(j, tmp_list.at(j));
        }
        ui->treeW_finsh_timer->addTopLevelItem(child);
    }
    QString tmp = list->last();
    tmp.remove(0, 13);
    ui->lineE_current_tick->setText(tmp);
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
        case 4:
            seimey_serial_c->serial_send_data("list_device\r\n");
            break;
        case 5:
            seimey_serial_c->serial_send_data("list_timer\r\n");
            break;
        }
    }
}

enum enum_mem_mode
{
    MEM_HEAP = 0,
    MEM_POOL,
    MEM_FREE
};
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
        break;
    case MEM_HEAP:
        ui->comdLB_mem_heap->setChecked(true);
        ui->comdLB_mem_free->setChecked(false);
        ui->comdLB_mem_pool->setChecked(false);
        break;
    case MEM_FREE:
        ui->comdLB_mem_free->setChecked(true);
        ui->comdLB_mem_pool->setChecked(false);
        ui->comdLB_mem_heap->setChecked(false);
        break;
    }
}

enum enum_synchronize_mode
{
    SYNCHR_SEM = 0,
    SYNCHR_MUTEX,
    SYNCHR_EVENT
};

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
        break;
    case SYNCHR_MUTEX:
        ui->comdLB_synchronize_mutex->setChecked(true);
        ui->comdLB_synchronize_semaphore->setChecked(false);
        ui->comdLB_synchronize_event->setChecked(false);
        break;
    case SYNCHR_EVENT:
        ui->comdLB_synchronize_event->setChecked(true);
        ui->comdLB_synchronize_semaphore->setChecked(false);
        ui->comdLB_synchronize_mutex->setChecked(false);
        break;
    }
}

enum enum_communicate_mode
{
    COMMUN_MAIL = 0,
    COMMUN_QUEUE,
};
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
        break;
    case COMMUN_QUEUE:
        ui->comdLB_communicate_queue->setChecked(true);
        ui->comdLB_communicate_mail->setChecked(false);
        break;
    }
}
