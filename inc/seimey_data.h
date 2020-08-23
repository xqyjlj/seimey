#ifndef SEIMEY_DATA_H
#define SEIMEY_DATA_H
#include <QDebug>

void seimey_create_workspace(void);
void seimey_msgoutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
bool seimey_get_is_save_serial_data(void);
double seimey_get_timed_refresh_time(void);
#endif // SEIMEY_DATA_H
