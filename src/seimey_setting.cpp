#include "seimey_setting.h"
#include "ui_seimey_setting.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QDoubleValidator>
seimey_setting::seimey_setting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::seimey_setting)
{
    ui->setupUi(this);
    set_setting();
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    ui->lineE_timer_refresh->setValidator(new QDoubleValidator(0, 10000, 1, this));
}

seimey_setting::~seimey_setting()
{
    delete ui;
}

void seimey_setting::closeEvent(QCloseEvent *event)
{
    event->accept();
    QJsonObject setting_json_obj;
    setting_json_obj.insert("Save serial", QJsonValue(ui->checkB_Save_serial_data->isChecked()));
    setting_json_obj.insert("Timed refresh", QJsonValue(ui->lineE_timer_refresh->text().toDouble()));

    QJsonDocument setting_json;
    setting_json.setObject(setting_json_obj);

    QString qs_setting_file = QDir::currentPath() + "/.workspace/.setting/setting.json";
    QFile setting_file(qs_setting_file);
    setting_file.open(QIODevice::ReadWrite | QIODevice::Truncate);

    unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    setting_file.write((char *)bom, sizeof(bom));
    QTextStream in(&setting_file);
    in.setCodec("UTF-8");
    in << QString(setting_json.toJson());

    setting_file.flush();
    setting_file.close();

    emit window_close();
}

void seimey_setting::set_setting(void)
{
    QJsonParseError jsonError;

    QString qs_setting_file = QDir::currentPath() + "/.workspace/.setting/setting.json";
    QFile setting_file(qs_setting_file);
    setting_file.open(QIODevice::ReadOnly);

    QJsonDocument setting_json = QJsonDocument::fromJson(setting_file.readAll(), &jsonError);
    if (!setting_json.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject setting_json_obj = setting_json.object();
        ui->checkB_Save_serial_data->setChecked(setting_json_obj.value("Save serial").toBool());
        ui->lineE_timer_refresh->setText(QString::number(setting_json_obj.value("Timed refresh").toDouble()));
    }

    setting_file.close();

}
