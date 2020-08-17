#include "seimey_setting.h"
#include "ui_seimey_setting.h"

seimey_setting::seimey_setting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::seimey_setting)
{
    ui->setupUi(this);
}

seimey_setting::~seimey_setting()
{
    delete ui;
}
