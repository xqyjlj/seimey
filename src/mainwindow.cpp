#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    seimey_init_CTL();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::seimey_init_CTL(void)
{
    this->ui->scrollA_task_communicate->setBackgroundRole(QPalette::Light); /* set the task_manager's communicate QScrollArea background (White) */
    this->ui->scrollA_task_synchronize->setBackgroundRole(QPalette::Light); /* set the task_manager's synchronize QScrollArea background (White) */
    this->ui->scrollA_task_property->setBackgroundRole(QPalette::Light); /* set the task_manager's property QScrollArea background (White) */

}
