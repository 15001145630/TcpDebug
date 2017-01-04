#-------------------------------------------------
#
# Project created by QtCreator 2016-12-25T09:15:46
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpDebug
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    common.cpp \
    clientmanage.cpp \
    tcpserver.cpp \
    tcpthread.cpp

HEADERS  += widget.h \
    common.h \
    clientmanage.h \
    tcpserver.h \
    tcpthread.h

FORMS    += widget.ui
