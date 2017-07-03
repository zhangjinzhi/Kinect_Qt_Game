#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T11:29:15
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app


SOURCES += main.cpp\
    GlobalFunction.cpp \
    GUI_BubbleWidget.cpp \
    chat_server.cpp \
    my_socket.cpp \
    thread.cpp

HEADERS  += \
    GlobalFunction.h \
    GUI_BubbleWidget.h \
    chat_server.h \
    thread.h \
    my_socket.h

FORMS    += mainwindow.ui

RESOURCES += \
    gui.qrc
