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
    src\seimey_qwchse.cpp \
    src\seimey_serial.cpp \
    src\main.cpp \
    src\mainwindow.cpp


HEADERS += \
    inc\mainwindow.h \
    inc\seimey_serial.h \
    inc\seimey_qwchse.h \

FORMS += \
    mainwindow.ui \
    seimey_qwchse.ui

TRANSLATIONS += \
    seimey_zh_CN.ts

INCLUDEPATH += \
    inc/

RC_FILE = seimey.rc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    seimey.qrc

DISTFILES += \
    seimey.rc
