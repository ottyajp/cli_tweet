QT += core network
QT -= gui

LIBS += -lcrypto

TARGET = cli_twitter
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    twit_func.cpp

HEADERS += \
    twit_func.h

