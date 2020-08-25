#ifndef SEIMEY_DATA_H
#define SEIMEY_DATA_H
#include <QDebug>

void seimey_Create_Dir(void);/* 创建目录 */
void seimey_Msg_Output(QtMsgType type, const QMessageLogContext &context, const QString &msg);/* 改变日志输出路径 */
bool seimey_Get_Is_Save_Serial_Data(void);/* 获得是否保存串口数据的标志位 */
double seimey_Get_Timed_Refresh_Time(void);/* 获得定时器刷新时间 */
void seimey_Save_Serial_Data(QString string);/* 保存串口数据 */
#endif // SEIMEY_DATA_H
