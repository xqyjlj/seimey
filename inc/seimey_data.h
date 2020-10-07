/*
* 数据处理
*
* 修改日志:
*
* 日期            作者          Notes
* 2020-08-17     xqyjlj       0.0.0     ：创建项目
*  ... ...     (中间日志丢失)
* 2020-08-25     xqyjlj       0.0.8     ：创建插件管理器
*/
#ifndef SEIMEY_DATA_H
#define SEIMEY_DATA_H
#include <QDebug>
#include <QObject>

class seimey_data : public QObject
{
    Q_OBJECT
public:
    explicit seimey_data(QObject *parent = nullptr);
public:
    void create_Dir(void);/* 创建目录 */
    bool get_Is_Save_Serial_Data(void);/* 获得是否保存串口数据的标志位 */
    double get_Timed_Refresh_Time(void);/* 获得定时器刷新时间 */
    void save_Serial_Data(QByteArray msg);/* 保存串口数据 */
signals:

public slots:

private:
    void ceate_Serial(void);
    void ceate_Log(void);
    void ceate_Setting(void);
    void create_Workspace(void);
private:

};



#endif // SEIMEY_DATA_H
