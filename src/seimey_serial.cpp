#include "seimey_serial.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QMutex>
/* serial base info  */
typedef struct
{
    QString SerialPort;
    QString BaudRate;
    QString FlowControl;
    QString Parity;
    QString StopBits;
    QString DataBits;
} serial_baseinfo_t;


static QSerialPort *serialport;/* serial object */
static serial_baseinfo_t serial_baseinfo;/* serial base info  */

seimey_serial::seimey_serial(QObject *parent) : QObject(parent)
{
    serialport = new QSerialPort(this);
    connect(serialport, SIGNAL(readyRead()), this, SLOT(serial_read_data()), Qt::UniqueConnection);
    connect(serialport, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(serial_error(QSerialPort::SerialPortError)), Qt::UniqueConnection);
}

void get_serial_info(serial_baseinfo_t *baseinfo)
{
    QJsonParseError jsonError;

    QString qs_serial_file = QDir::currentPath() + "/.workspace/.serial/serial.json";
    QFile serial_file(qs_serial_file);
    serial_file.open(QIODevice::ReadOnly);

    QJsonDocument serial_json = QJsonDocument::fromJson(serial_file.readAll(), &jsonError);
    if (!serial_json.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject serial_json_obj = serial_json.object();
        baseinfo->BaudRate = serial_json_obj.value("BaudRate").toString();
        baseinfo->Parity = serial_json_obj.value("Parity").toString();
        baseinfo->StopBits = serial_json_obj.value("StopBits").toString();
        baseinfo->FlowControl = serial_json_obj.value("FlowControl").toString();
        baseinfo->DataBits = serial_json_obj.value("DataBits").toString();
        baseinfo->SerialPort = serial_json_obj.value("SerialPort").toString();
    }
    serial_file.close();
}


static void open_serial(serial_baseinfo_t *baseinfo)
{
    if (!serialport->isOpen())
    {
        serialport->setPortName(baseinfo->SerialPort);

        serialport->setBaudRate(baseinfo->BaudRate.toInt());

        QSerialPort::DataBits DataBits;
        switch (baseinfo->DataBits.toInt())
        {
        case 8:
            DataBits = QSerialPort::Data8;
            break;
        case 7:
            DataBits = QSerialPort::Data7;
            break;
        case 6:
            DataBits = QSerialPort::Data6;
            break;
        case 5:
            DataBits = QSerialPort::Data5;
            break;
        default:
            DataBits = QSerialPort::Data8;
            break;
        }
        serialport->setDataBits(DataBits);

        QSerialPort::Parity Parity;
        if (baseinfo->Parity == QString("无校验"))
            Parity = QSerialPort::NoParity;
        else if (baseinfo->Parity == QString("奇校验"))
            Parity = QSerialPort::OddParity;
        else if (baseinfo->Parity == QString("偶校验"))
            Parity = QSerialPort::EvenParity;
        else if (baseinfo->Parity == QString("1校验"))
            Parity = QSerialPort::MarkParity;
        else if (baseinfo->Parity == QString("0校验"))
            Parity = QSerialPort::SpaceParity;
        else
            Parity = QSerialPort::NoParity;
        serialport->setParity(Parity);

        QSerialPort::StopBits StopBits;
        if (baseinfo->StopBits == QString("1.0"))
            StopBits = QSerialPort::OneStop;
        else if (baseinfo->StopBits == QString("1.0"))
            StopBits = QSerialPort::OneAndHalfStop;
        else if (baseinfo->StopBits == QString("1.0"))
            StopBits = QSerialPort::TwoStop;
        else
            StopBits = QSerialPort::OneStop;
        serialport->setStopBits(StopBits);

        QSerialPort::FlowControl FlowControl;
        if (baseinfo->FlowControl == QString("无流控"))
            FlowControl = QSerialPort::NoFlowControl;
        else if (baseinfo->FlowControl == QString("硬流控"))
            FlowControl = QSerialPort::HardwareControl;
        else if (baseinfo->FlowControl == QString("软流控"))
            FlowControl = QSerialPort::SoftwareControl;
        else
            FlowControl = QSerialPort::NoFlowControl;
        serialport->setFlowControl(FlowControl);
        serialport->open(QIODevice::ReadWrite);
    }

}

static void close_serial(void)
{
    if (serialport->isOpen())
    {
        serialport->clear();
        serialport->close();
    }
}


bool seimey_serial::set_serial_link(bool status)
{
    static QMutex mutex;
    mutex.lock();
    get_serial_info(&serial_baseinfo);
    if (status)
    {
        open_serial(&serial_baseinfo);
    }
    else
    {
        close_serial();
    }
    mutex.unlock();
    return serialport->isOpen();
}

QString seimey_serial::get_seial_port(void)
{
    return serial_baseinfo.SerialPort;
}

QString seimey_serial::get_seial_baud(void)
{
    return serial_baseinfo.BaudRate;
}
void seimey_serial::serial_read_data(void)
{
    static QMutex mutex;
    mutex.lock();
    QByteArray info = serialport->readAll();
    msg += info;
    while (msg.contains("\n"))
    {
        if (msg.contains("\n"))
        {
            int len = msg.indexOf("\n");
            if (len > -1)
            {
                QByteArray _msg = msg.left(len + 1);
                if (_msg.contains("\r\n"))
                {
                    _msg = _msg.left(_msg.length() - 2);
                }
                msg.remove(0, len + 1);
                emit already_recv_data(_msg);
            }
        }
    }
    mutex.unlock();
}

void seimey_serial::serial_error(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::DeviceNotFoundError)
    {
        QMessageBox::warning(nullptr, "警告", "未知的设备，请刷新设备表");
    }
    else if (error == QSerialPort::OpenError)
    {
        QMessageBox::warning(nullptr, "警告", "重复打开串口设备");
    }
    else if (error == QSerialPort::PermissionError)
    {
        QMessageBox::warning(nullptr, "警告", "此串口已被其他应用程序使用");
    }
    else if (error == QSerialPort::WriteError)
    {
        QMessageBox::warning(nullptr, "警告", "串口写入错误");
    }
}

void seimey_serial::send_data(const char *data)
{
    if (serialport->isOpen())
    {
        serialport->write(data);
    }
}

bool seimey_serial::get_serial_status(void)
{
    return serialport->isOpen();
}

void seimey_serial::send_data(QByteArray byteArray)
{
    if (serialport->isOpen())
    {
        serialport->write(byteArray);
    }
}
