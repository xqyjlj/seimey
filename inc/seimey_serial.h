#ifndef SEIMEY_SERIAL_H
#define SEIMEY_SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QQueue>
class seimey_serial : public QObject
{
    Q_OBJECT
public:
    explicit seimey_serial(QObject *parent = nullptr);

    bool set_serial_link(bool status);
    QString get_seial_port(void);
    QString get_seial_baud(void);
    void send_data(const char *data);
    void send_data(QByteArray byteArray);
    bool get_serial_status();

signals:
    void already_recv_data(QByteArray msg);

private slots:
    void serial_read_data(void);
    void serial_error(QSerialPort::SerialPortError error);
private:
    QByteArray msg;
};

#endif // SEIMEY_SERIAL_H
