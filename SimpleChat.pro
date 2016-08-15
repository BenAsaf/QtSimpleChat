#-------------------------------------------------
#
# Project created by QtCreator 2016-04-19T18:46:11
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleChat
TEMPLATE = app
CONFIG += static

SOURCES += main.cpp\
        mainwindow.cpp \
    Dialogs/Dialog_About.cpp \
    Dialogs/Dialog_HostSession.cpp \
    Dialogs/Dialog_JoinSession.cpp \
    TcpServer.cpp \
    TcpClient.cpp \
    TcpConnection.cpp

HEADERS  += mainwindow.h \
    Dialogs/Dialog_About.h \
    Dialogs/Dialog_HostSession.h \
    Dialogs/Dialog_JoinSession.h \
    TcpServer.h \
    TcpClient.h \
    TcpConnection.h

FORMS    += mainwindow.ui \
    Dialogs/Dialog_About.ui \
    Dialogs/Dialog_Hostsession.ui \
    Dialogs/Dialog_Joinsession.ui
