#include "seimey_text.h"
#include "ui_seimey_text.h"

seimey_text::seimey_text(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::seimey_text)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_QuitOnClose, false);
}

seimey_text::~seimey_text()
{
    delete ui;
}
void seimey_text::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit window_Close();
}

void seimey_text::show_text(QString msg)
{
    ui->textEdit->setText(msg);
}
