#ifndef SEIMEY_SERIAL_H
#define SEIMEY_SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
class seimey_serial : public QObject
{
    Q_OBJECT
public:
    explicit seimey_serial(QObject *parent = nullptr);

    bool set_serial_link(bool status);
    QString get_seial_port(void);
    QString get_seial_baud(void);

signals:

private slots:
    void serial_read_data(void);
    void serial_error(QSerialPort::SerialPortError error);
};

#endif // SEIMEY_SERIAL_H
