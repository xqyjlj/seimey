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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDateTime>
#include <QInputDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    c_Seimey_Data->create_Dir();
    init_ctl();
    init_connect();
    setAttribute(Qt::WA_QuitOnClose, true);
    qInfo() << "open";

    QList<int> list;

    list <<12 <<13<<16<<10;

    qSort(list.begin(), list.end());
    qDebug() << list;
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
    setting.is_save_serial = c_Seimey_Data->get_Is_Save_Serial_Data();
    setting.timed_refresh = c_Seimey_Data->get_Timed_Refresh_Time();
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
    connect(finsh_timerID, SIGNAL(timeout()), this, SLOT(finsh_timer_timeout()), Qt::UniqueConnection);
    if (setting.timed_refresh > 0)
    {
        if (ui->tabW_main->currentIndex() == 0)
        {
            finsh_timerID->setInterval(static_cast<int>(setting.timed_refresh * 1000));
            finsh_timerID->start();
        }
    }
    QHeaderView *head = ui->treeW_file_mannage->header();
    head->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->treeW_finsh_thread->horizontalHeader(),&QHeaderView::sectionClicked, this,&MainWindow::sortHeader);
}
/*
 *  用来初始化一些connect函数
*/
void MainWindow::init_connect(void)
{
    connect(c_Seimey_Serial, SIGNAL(already_recv_data(QByteArray)), this, SLOT(serial_data_coming(QByteArray)), Qt::UniqueConnection);
    connect(c_Seimey_Plugin, SIGNAL(send_msg(QByteArray)), this, SLOT(send_msg(QByteArray)), Qt::UniqueConnection);
    connect(ui->treeW_file_mannage, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTreeRightMenu(QPoint)), Qt::UniqueConnection);
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
        c_Seimey_Qwchse = nullptr;
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
        setting.is_save_serial = c_Seimey_Data->get_Is_Save_Serial_Data();
        setting.timed_refresh = c_Seimey_Data->get_Timed_Refresh_Time();
        /* 如果启动了定时刷新，则会启动定时器 */
        if (setting.timed_refresh > 0)
        {
            if (ui->tabW_main->currentIndex() == 0)
            {
                finsh_timerID->setInterval(static_cast<int>(setting.timed_refresh * 1000));
                finsh_timerID->start();
            }
        }
        else
        {
            finsh_timerID->stop();
        }
        c_Seimey_Setting = nullptr;
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
 * 串口数据来临槽函数
*/
void MainWindow::serial_data_coming(QByteArray msg)
{
    if (setting.is_save_serial)
    {
        c_Seimey_Data->save_Serial_Data(msg);
    }
    if (c_Seimey_Plugin != nullptr)
    {
        c_Seimey_Plugin->rec_msg(msg);
    }
    switch (ui->tabW_main->currentIndex())
    {
    case 0:
        c_Seimey_Finsh->handle(QString::fromLocal8Bit(msg));
        break;
    case 1:
        c_Seimey_Dfs->handle(QString::fromLocal8Bit(msg));
        break;
    default:
        break;
    }
}
/*
 * 任务管理器顶端标签改变槽函数
*/
void MainWindow::task_manager(int index)
{
    if (c_Seimey_Serial->get_serial_status())
    {
        switch (index)
        {
        case 0:
            c_Seimey_Finsh->thread(c_Seimey_Serial, ui->treeW_finsh_thread);
            break;
        case 1:
            if (ui->comdLB_mem_pool->isChecked())
                c_Seimey_Finsh->mem_pool(c_Seimey_Serial, ui->treeW_finsh_mempool);
            else if (ui->comdLB_mem_heap->isChecked())
                c_Seimey_Finsh->mem_heap(c_Seimey_Serial, ui->treeW_finsh_memheap);
            else if (ui->comdLB_mem_free->isChecked())
                c_Seimey_Finsh->mem_free(c_Seimey_Serial, ui->proW_memfree, ui->label_total_mem, ui->label_maxused_mem, ui->label_remaining_mem);
            break;
        case 2:
            if (ui->comdLB_synchronize_event->isChecked())
                c_Seimey_Finsh->synchr_event(c_Seimey_Serial, ui->treeW_finsh_event);
            else if (ui->comdLB_synchronize_mutex->isChecked())
                c_Seimey_Finsh->synchr_mutex(c_Seimey_Serial, ui->treeW_finsh_mutex);
            else if (ui->comdLB_synchronize_semaphore->isChecked())
                c_Seimey_Finsh->synchr_sem(c_Seimey_Serial, ui->treeW_finsh_sem);
            break;
        case 3:
            if (ui->comdLB_communicate_mail->isChecked())
                c_Seimey_Finsh->commun_mail(c_Seimey_Serial, ui->treeW_finsh_mailbox);
            else if (ui->comdLB_communicate_queue->isChecked())
                c_Seimey_Finsh->commun_queue(c_Seimey_Serial, ui->treeW_finsh_msqueue);
            break;
        case 4:
            c_Seimey_Finsh->device(c_Seimey_Serial, ui->treeW_finsh_device);
            break;
        case 5:
            c_Seimey_Finsh->time(c_Seimey_Serial, ui->treeW_finsh_timer, ui->lineE_current_tick);
            break;
        }
    }
}
/*
 * 内存池按钮点击槽函数
*/
void MainWindow::on_comdLB_mem_pool_clicked(void)
{
    ui->comdLB_mem_pool->setChecked(true);
    ui->comdLB_mem_free->setChecked(false);
    ui->comdLB_mem_heap->setChecked(false);
    ui->stackedW_property->setCurrentIndex(0);
}
/*
 * 内存堆按钮点击槽函数
*/
void MainWindow::on_comdLB_mem_heap_clicked(void)
{
    ui->comdLB_mem_heap->setChecked(true);
    ui->comdLB_mem_free->setChecked(false);
    ui->comdLB_mem_pool->setChecked(false);
    ui->stackedW_property->setCurrentIndex(1);
}
/*
 * 动态按钮点击槽函数
*/
void MainWindow::on_comdLB_mem_free_clicked(void)
{
    ui->comdLB_mem_free->setChecked(true);
    ui->comdLB_mem_pool->setChecked(false);
    ui->comdLB_mem_heap->setChecked(false);
    ui->stackedW_property->setCurrentIndex(2);
}
/*
 * 信号量按钮点击槽函数
*/
void MainWindow::on_comdLB_synchronize_semaphore_clicked()
{
    ui->comdLB_synchronize_semaphore->setChecked(true);
    ui->comdLB_synchronize_mutex->setChecked(false);
    ui->comdLB_synchronize_event->setChecked(false);
    ui->stackedW_synchronize->setCurrentIndex(0);
}
/*
 * 二值信号量按钮点击槽函数
*/
void MainWindow::on_comdLB_synchronize_mutex_clicked()
{
    ui->comdLB_synchronize_mutex->setChecked(true);
    ui->comdLB_synchronize_semaphore->setChecked(false);
    ui->comdLB_synchronize_event->setChecked(false);
    ui->stackedW_synchronize->setCurrentIndex(1);
}
/*
 * 事件按钮点击槽函数
*/
void MainWindow::on_comdLB_synchronize_event_clicked()
{
    ui->comdLB_synchronize_event->setChecked(true);
    ui->comdLB_synchronize_semaphore->setChecked(false);
    ui->comdLB_synchronize_mutex->setChecked(false);
    ui->stackedW_synchronize->setCurrentIndex(2);
}
/*
 * 邮箱按钮点击槽函数
*/
void MainWindow::on_comdLB_communicate_mail_clicked()
{
    ui->comdLB_communicate_mail->setChecked(true);
    ui->comdLB_communicate_queue->setChecked(false);
    ui->stackedW_communicate->setCurrentIndex(1);
}
/*
 * 队列按钮点击槽函数
*/
void MainWindow::on_comdLB_communicate_queue_clicked()
{
    ui->comdLB_communicate_queue->setChecked(true);
    ui->comdLB_communicate_mail->setChecked(false);
    ui->stackedW_communicate->setCurrentIndex(0);
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
    if (ui->tabW_main->currentIndex() != 0)
    {
        finsh_timerID->stop();
    }
    else
    {
        task_manager(ui->tabW_task_manager->currentIndex());
    }
}

void MainWindow::on_tabW_main_currentChanged(int index)
{
    on_tabW_main_tabBarDoubleClicked(index);
}

void MainWindow::on_tabW_main_tabBarDoubleClicked(int index)
{
    switch (index)
    {
    case 0:
        setting.timed_refresh = c_Seimey_Data->get_Timed_Refresh_Time();
        if (setting.timed_refresh > 0)
        {
            finsh_timerID->setInterval(static_cast<int>(setting.timed_refresh * 1000));
            finsh_timerID->start();
        }
        else
        {
            finsh_timerID->stop();
        }
        break;
    case 1:
        c_Seimey_Dfs->master(c_Seimey_Serial, ui->treeW_file_mannage);
        break;
    case 2:
        c_Seimey_Plugin->set_plugin(ui->treeW_plugin);
        break;
    default:
        break;
    }
}

void MainWindow::on_treeW_file_mannage_itemDoubleClicked(QTreeWidgetItem *item)
{
    c_Seimey_Dfs->servant(c_Seimey_Serial, item);
}

void MainWindow::showTreeRightMenu(QPoint pos)
{
    dfs_menu->clear();
    treew_dfs_MenuRequested = ui->treeW_file_mannage->itemAt(pos);//关键
    if (treew_dfs_MenuRequested)
    {
        switch (treew_dfs_MenuRequested->type())
        {
        case seimey_dfs::enumdir:
            dfs_menu->addAction(ui->dfs_menu_open);
            dfs_menu->addSeparator();
            dfs_menu->addAction(ui->dfs_menu_mkdir);
            dfs_menu->addAction(ui->dfs_menu_echo);
            dfs_menu->addSeparator();
            dfs_menu->addAction(ui->dfs_menu_rm);
            dfs_menu->addSeparator();
            dfs_menu->addAction(ui->dfs_menu_clone);
            dfs_menu->addSeparator();
            dfs_menu->addAction(ui->dfs_menu_copy);
            if (!dfs_clipboard.isEmpty())
            {
                dfs_menu->addAction(ui->dfs_menu_paste);
            }
            dfs_menu->addSeparator();
            break;
        case seimey_dfs::enumfile:
            dfs_menu->addAction(ui->dfs_menu_open);
            dfs_menu->addSeparator();
            dfs_menu->addAction(ui->dfs_menu_rm);
            dfs_menu->addSeparator();
            dfs_menu->addAction(ui->dfs_menu_clone);
            dfs_menu->addSeparator();
            dfs_menu->addAction(ui->dfs_menu_copy);
            dfs_menu->addSeparator();
            break;
        }
    }
    else
    {
        dfs_menu->addAction(ui->dfs_menu_mkdir_2);
        dfs_menu->addAction(ui->dfs_menu_echo_2);
        dfs_menu->addSeparator();
        if (!dfs_clipboard.isEmpty())
        {
            dfs_menu->addAction(ui->dfs_menu_paste_2);
        }
        dfs_menu->addAction(ui->dfs_menu_clone_2);

    }
    dfs_menu->move(ui->treeW_file_mannage->cursor().pos());
    dfs_menu->show();
}

void MainWindow::on_dfs_menu_open_triggered()
{
    if (treew_dfs_MenuRequested)
    {
        c_Seimey_Dfs->servant(c_Seimey_Serial, treew_dfs_MenuRequested);
        treew_dfs_MenuRequested = nullptr;
    }
}


void MainWindow::on_dfs_menu_clone_triggered()
{
    QMessageBox::information(this, "信息", "该功能还未实现");
}

void MainWindow::on_dfs_menu_mkdir_triggered()
{
    if (treew_dfs_MenuRequested)
    {
        QString dlgTitle = "新建文件夹";
        QString txtLabel = "请输入文件夹名称，不能含有空格，输入的空格将会用'_'代替";
        QString defaultInput = "";
        QLineEdit::EchoMode echoMode = QLineEdit::Normal; //正常文字输入
        //QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入
        bool ok = false;
        QString msg = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, defaultInput, &ok, Qt::MSWindowsFixedSizeDialogHint);
        if (ok && !msg.isEmpty())
        {
            if (msg.contains(" "))
            {
                msg.replace(QString(" "), QString("_"));
            }
            c_Seimey_Dfs->mkdir(c_Seimey_Serial, msg, treew_dfs_MenuRequested);
            treew_dfs_MenuRequested = nullptr;
        }
    }
}

void MainWindow::on_dfs_menu_echo_triggered()
{
    if (treew_dfs_MenuRequested)
    {
        QDialog dialog(this);
        dialog.setMinimumSize(200, 200);
        QFormLayout form(&dialog);
        form.addRow(new QLabel("输入的字符不能含有空格，输入的空格将会用'_'代替"));
        QLineEdit *filename = new QLineEdit(&dialog);
        form.addRow(QString("文件名"), filename);
        QLineEdit *filedata = new QLineEdit(&dialog);
        form.addRow(QString("内容"), filedata);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()), Qt::UniqueConnection);
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()), Qt::UniqueConnection);

        if (dialog.exec() == QDialog::Accepted)
        {
            QString name = filename->text();
            QString msg = filedata->text();
            if (name.isEmpty())
            {
                QMessageBox::warning(nullptr, "警告", "文件名未输入");
                return;
            }
            else if (msg.isEmpty())
            {
                QMessageBox::warning(nullptr, "警告", "文件内容未输入");
                return;
            }

            if (!name.isEmpty() && !msg.isEmpty())
            {
                if (name.contains(" "))
                {
                    name.replace(QString(" "), QString("_"));
                }
                if (msg.contains(" "))
                {
                    msg.replace(QString(" "), QString("_"));
                }
                c_Seimey_Dfs->echo(c_Seimey_Serial, name, msg, treew_dfs_MenuRequested);
                treew_dfs_MenuRequested = nullptr;
            }
        }
    }
}

void MainWindow::on_dfs_menu_rm_triggered()
{
    if (treew_dfs_MenuRequested)
    {
        c_Seimey_Dfs->rm(c_Seimey_Serial, treew_dfs_MenuRequested, ui->treeW_file_mannage);
        treew_dfs_MenuRequested = nullptr;
    }
}

void MainWindow::on_dfs_menu_copy_triggered()
{
    if (treew_dfs_MenuRequested)
    {
        dfs_clipboard = treew_dfs_MenuRequested->whatsThis(0) + treew_dfs_MenuRequested->text(0);
        treew_dfs_MenuRequested = nullptr;
    }
}

void MainWindow::on_dfs_menu_paste_triggered()
{
    if (treew_dfs_MenuRequested)
    {
        QString pur = treew_dfs_MenuRequested->whatsThis(0) + treew_dfs_MenuRequested->text(0);
        if (pur == dfs_clipboard)
        {
            pur += QString("x");
        }
        c_Seimey_Dfs->cp(c_Seimey_Serial, dfs_clipboard, pur, treew_dfs_MenuRequested, ui->treeW_file_mannage);
        dfs_clipboard.clear();
        treew_dfs_MenuRequested = nullptr;
    }
}

void MainWindow::on_dfs_menu_clone_2_triggered()
{
    QMessageBox::information(this, "信息", "该功能还未实现");
}

void MainWindow::on_dfs_menu_mkdir_2_triggered()
{
    QString dlgTitle = "新建文件夹";
    QString txtLabel = "请输入文件夹名称，不能含有空格，输入的空格将会用'_'代替";
    QString defaultInput = "";
    QLineEdit::EchoMode echoMode = QLineEdit::Normal; //正常文字输入
    //QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入
    bool ok = false;
    QString msg = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, defaultInput, &ok, Qt::MSWindowsFixedSizeDialogHint);
    if (ok && !msg.isEmpty())
    {
        if (msg.contains(" "))
        {
            msg.replace(QString(" "), QString("_"));
        }
        c_Seimey_Dfs->mastermkdir(c_Seimey_Serial, msg, ui->treeW_file_mannage);
    }
}

void MainWindow::on_dfs_menu_echo_2_triggered()
{
    QDialog dialog(this);
    dialog.setMinimumSize(200, 200);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("输入的字符不能含有空格，输入的空格将会用'_'代替"));
    QLineEdit *filename = new QLineEdit(&dialog);
    form.addRow(QString("文件名"), filename);
    QLineEdit *filedata = new QLineEdit(&dialog);
    form.addRow(QString("内容"), filedata);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()), Qt::UniqueConnection);
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()), Qt::UniqueConnection);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString name = filename->text();
        QString msg = filedata->text();
        if (name.isEmpty())
        {
            QMessageBox::warning(nullptr, "警告", "文件名未输入");
            return;
        }
        else if (msg.isEmpty())
        {
            QMessageBox::warning(nullptr, "警告", "文件内容未输入");
            return;
        }

        if (!name.isEmpty() && !msg.isEmpty())
        {
            if (name.contains(" "))
            {
                name.replace(QString(" "), QString("_"));
            }
            if (msg.contains(" "))
            {
                msg.replace(QString(" "), QString("_"));
            }
            c_Seimey_Dfs->masterecho(c_Seimey_Serial, name, msg, ui->treeW_file_mannage);
        }
    }
}

void MainWindow::on_dfs_menu_paste_2_triggered()
{
    c_Seimey_Dfs->mastercp(c_Seimey_Serial, dfs_clipboard, ui->treeW_file_mannage);
    dfs_clipboard.clear();
}

void MainWindow::on_treeW_plugin_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (column >= 0)
    {
        c_Seimey_Plugin->open_plugin(item);
    }
}

void MainWindow::send_msg(QByteArray msg)
{
    c_Seimey_Serial->send_data(msg);
}

void MainWindow::sortHeader(int index)
{
    ui->treeW_finsh_thread->sortItems(index, Qt::AscendingOrder);
}
