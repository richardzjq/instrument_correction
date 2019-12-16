#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T18:38:15
#
#-------------------------------------------------

QT       += core gui sql xml network axcontainer

greaterThan(QT_MAJOR_VERSION, 4){
QT += widgets printsupport multimedia multimediawidgets designer
} else {
QT += phonon
CONFIG += designer
}

#如果改成tool则编译出来的是设备模拟工具
DEFINES             += tool1 ssl
contains(DEFINES, tool) {
TARGET              = inspetortool
RC_ICONS            = other/tool.ico
} else {
TARGET              = inspetor
RC_ICONS            = other/main.ico
}

VERSION             = 2019.12.15
TEMPLATE            = app
MOC_DIR             = temp/moc
RCC_DIR             = temp/rcc
UI_DIR              = temp/ui
OBJECTS_DIR         = temp/obj
DESTDIR             = bin
PRECOMPILED_HEADER  = head.h

CONFIG              += warn_off resources_big
SOURCES             += main.cpp
RESOURCES           += other/main.qrc
RESOURCES           += other/qss.qrc

INCLUDEPATH         += $$PWD/frmmain
INCLUDEPATH         += $$PWD/frminspect
INCLUDEPATH         += $$PWD/frmconfig

INCLUDEPATH         += $$PWD
INCLUDEPATH         += $$PWD/api
INCLUDEPATH         += $$PWD/dbapi
INCLUDEPATH         += $$PWD/dataout
INCLUDEPATH         += $$PWD/usercontrol
INCLUDEPATH         += $$PWD/qextserialport

win32 {
    INCLUDEPATH += "C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Include"
    LIBS += "C:\Program Files (x86)\IVI Foundation\VISA\WinNT\lib\msc\visa32.lib"
}

include             ($$PWD/frmmain/frmmain.pri)
include             ($$PWD/frminspect/frminspect.pri)
include             ($$PWD/frmconfig/frmconfig.pri)

include             ($$PWD/api/api.pri)
include             ($$PWD/dbapi/dbapi.pri)
include             ($$PWD/dataout/dataout.pri)
include             ($$PWD/usercontrol/usercontrol.pri)
include             ($$PWD/qextserialport/qextserialport.pri)
