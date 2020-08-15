#include "seimey_qwchse.h"
#include "ui_seimey_qwchse.h"
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include "QDebug"
seimey_qwchse::seimey_qwchse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::seimey_qwchse)
{
    ui->setupUi(this);
    set_serial_comboBox();
    connect(ui->label_serialport, SIGNAL(clicked()), this, SLOT(set_serial_comboBox()));
    set_excserial_comboBox();
}

seimey_qwchse::~seimey_qwchse()
{
    delete ui;
}

void seimey_qwchse::set_serial_comboBox()
{
    QStringList serialPortName;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serialPortName << info.portName();
    }
    ui->comb_serialport->clear();
    ui->comb_serialport->addItems(serialPortName);
}

void seimey_qwchse::closeEvent(QCloseEvent *event)
{
    event->accept();

    /* create serial json object */
    QJsonObject serial_json_obj;
    serial_json_obj.insert("SerialPort", ui->comb_serialport->currentText());
    serial_json_obj.insert("BaudRate", ui->comb_baudrate->currentText());
    serial_json_obj.insert("DataBits", ui->comb_databits->currentText());
    serial_json_obj.insert("FlowControl", ui->comb_flowcontrol->currentText());
    serial_json_obj.insert("Parity", ui->comb_parity->currentText());
    serial_json_obj.insert("StopBits", ui->comb_stopbits->currentText());

    QJsonDocument serial_json;
    serial_json.setObject(serial_json_obj);

    /* open serial json file */
    QString qs_serial_file = QDir::currentPath() + "/.workspace/.serial/serial.json";
    QFile serial_file(qs_serial_file);
    serial_file.open(QIODevice::ReadWrite | QIODevice::Truncate);

    /* write file bom and json data */
    unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    serial_file.write((char *)bom, sizeof(bom));
    QTextStream in(&serial_file);
    in.setCodec("UTF-8");
    in << QString(serial_json.toJson());

    /* close file */
    serial_file.close();

    emit window_close();
}

void seimey_qwchse::set_excserial_comboBox(void)
{
    QJsonParseError jsonError;

    QString qs_serial_file = QDir::currentPath() + "/.workspace/.serial/serial.json";
    QFile serial_file(qs_serial_file);
    serial_file.open(QIODevice::ReadOnly);

    QJsonDocument serial_json = QJsonDocument::fromJson(serial_file.readAll(), &jsonError);
    if (!serial_json.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject serial_json_obj = serial_json.object();
        ui->comb_serialport->setCurrentText(serial_json_obj.value("SerialPort").toString());
        ui->comb_baudrate->setCurrentText(serial_json_obj.value("BaudRate").toString());
        ui->comb_parity->setCurrentText(serial_json_obj.value("Parity").toString());
        ui->comb_stopbits->setCurrentText(serial_json_obj.value("StopBits").toString());
        ui->comb_flowcontrol->setCurrentText(serial_json_obj.value("FlowControl").toString());
        ui->comb_databits->setCurrentText(serial_json_obj.value("DataBits").toString());
    }

    serial_file.close();
}


seimey_qwchse_Label::seimey_qwchse_Label(QWidget *parent): QLabel(parent)
{
}

seimey_qwchse_Label::~seimey_qwchse_Label()
{
}

void seimey_qwchse_Label::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}
