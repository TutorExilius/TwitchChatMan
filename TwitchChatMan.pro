#-------------------------------------------------
#
# Project created by QtCreator 2018-01-25T20:05:56
#
#-------------------------------------------------

QT       += core gui widgets network

CONFIG += c++11
QMAKE_CXXFLAGS  += -std=c++11
QMAKE_LFLAGS  += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TwitchChatMan
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    checkablechatmessage.cpp \
    chatmessage.cpp \
    chatmanager.cpp \
    parser.cpp \
    ircchat.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    checkablechatmessage.h \
    chatmessage.h \
    chatmanager.h \
    parser.h \
    ircchat.h \
    config.h

FORMS    += mainwindow.ui \
    checkablechatmessage.ui
