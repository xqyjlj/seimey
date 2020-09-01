#ifndef SEIMEY_TEXT_H
#define SEIMEY_TEXT_H

#include <QMainWindow>
#include <QMouseEvent>
namespace Ui
{
class seimey_text;
}

class seimey_text : public QMainWindow
{
    Q_OBJECT

public:
    explicit seimey_text(QWidget *parent = nullptr);
    ~seimey_text();

    void show_text(QString msg);
signals:
    void window_Close(void);

protected:
    virtual void closeEvent(QCloseEvent *event);
private:
    Ui::seimey_text *ui;
};

#endif // SEIMEY_TEXT_H
