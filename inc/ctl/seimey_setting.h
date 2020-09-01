#ifndef SEIMEY_SETTING_H
#define SEIMEY_SETTING_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui
{
class seimey_setting;
}

class seimey_setting : public QMainWindow
{
    Q_OBJECT

public:
    explicit seimey_setting(QWidget *parent = nullptr);
    ~seimey_setting();

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::seimey_setting *ui;

    void set_setting(void);

signals:
    void window_close(void);
};

#endif // SEIMEY_SETTING_H
