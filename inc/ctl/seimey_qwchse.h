#ifndef SEIMEY_QWCHSE_H
#define SEIMEY_QWCHSE_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
namespace Ui
{
class seimey_qwchse;
}

class seimey_qwchse : public QMainWindow
{
    Q_OBJECT

public:
    explicit seimey_qwchse(QWidget *parent = nullptr);
    ~seimey_qwchse();

private slots:
    void set_serial_comboBox(void);

private:
    Ui::seimey_qwchse *ui;

    void set_excserial_comboBox(void);

protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void window_close(void);

};



class seimey_qwchse_Label : public QLabel
{
    Q_OBJECT
public:
    explicit seimey_qwchse_Label(QWidget *parent = nullptr);
    ~seimey_qwchse_Label();

protected:
    virtual void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(void);
};



#endif // SEIMEY_QWCHSE_H
