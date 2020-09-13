#-------------------------------------------------
#
# Project created by QtCreator 2020-08-14T14:37:27
#
#-------------------------------------------------


QT  += serialport
QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


TARGET = seimey
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src\seimey_plugin.cpp \
    src\ctl\seimey_text.cpp \
    src\seimey_dfs.cpp \
    src\seimey_finsh.cpp \
    src\seimey_data.cpp \
    src\ctl\seimey_setting.cpp \
    src\ctl\seimey_qwchse.cpp \
    src\seimey_serial.cpp \
    src\main.cpp \
    src\ctl\mainwindow.cpp

HEADERS += \
    inc\ctl\mainwindow.h \
    inc\seimey_serial.h \
    inc\ctl\seimey_qwchse.h \
    sdk\progresswater.h \
    sdk\navlabel.h\
    inc\ctl\seimey_setting.h \
    inc\seimey_data.h \
    inc\seimey_finsh.h \
    inc\seimey_dfs.h \
    inc\ctl\seimey_text.h \
    inc\seimey_plugin.h

FORMS += \
    ui\seimey_text.ui \
    ui\mainwindow.ui \
    ui\seimey_qwchse.ui \
    ui\seimey_setting.ui

TRANSLATIONS += \
    seimey_zh_CN.ts

INCLUDEPATH += \
    inc\
    inc\ctl\
    sdk\
    plugin\
    plugin\pack

QT += xml
CONFIG(debug, debug|release){
LIBS += -L$$PWD/sdk/ -lqucd
} else {
LIBS += -L$$PWD/sdk/ -lquc
}


RC_FILE = seimey.rc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    seimey.qrc

DISTFILES += \
    seimey.rc

DEFINES += QT_MESSAGELOGCONTEXT
