#ifndef GDUT_FSAE_INTERFACE_H
#define GDUT_FSAE_INTERFACE_H

#include <QString>
#include <QtPlugin>

class gdut_fsae_interface
{
public:
    virtual ~gdut_fsae_interface() {}

    virtual void show() = 0;

    virtual void rec_Msg(QString msg) = 0;/* 接收到来自创建管理器的消息 */

    virtual void plugin_Close(QString msg) = 0;
};

#define gdut_fsae_interface_iid "gdut_fsae_plugin.interface"

Q_DECLARE_INTERFACE(gdut_fsae_interface, gdut_fsae_interface_iid)



#endif // GDUT_FSAE_INTERFACE_H
