#ifndef SEIMEY_SETTING_H
#define SEIMEY_SETTING_H

#include <QMainWindow>

namespace Ui {
class seimey_setting;
}

class seimey_setting : public QMainWindow
{
    Q_OBJECT

public:
    explicit seimey_setting(QWidget *parent = nullptr);
    ~seimey_setting();

private:
    Ui::seimey_setting *ui;
};

#endif // SEIMEY_SETTING_H
