#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "seimey_serial.h"
#include "seimey_qwchse.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_menu_setting_serial_setting_triggered(void);
    void on_menu_setting_serial_link_triggered(void);
    void on_menu_help_about_triggered(void);
    void on_menu_help_github_triggered(void);

    void on_menu_setting_preference_triggered();

    void on_menu_setting_serial_setting_2_triggered();

private:
    Ui::MainWindow *ui;
    seimey_serial *seimey_serial_c = new seimey_serial(this);
    seimey_qwchse *seimey_qwchse_c;

    void init_CTL(void);
    void init_connect(void);
    void create_workspace(void);
};
#endif // MAINWINDOW_H
