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
    init_CTL();
    init_connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_CTL(void)
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
