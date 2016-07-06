#-------------------------------------------------
#
# Project created by QtCreator 2015-08-13T11:50:41
#
#-------------------------------------------------

QT       += core gui sql serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CANAnalyzer
TEMPLATE = app
RC_FILE = app.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    filter/createfilterdialog.cpp \
    caninterface.cpp \
    packetconverter.cpp \
    chronomodel.cpp \
    uniquemodel.cpp \
    hexvalidator.cpp \
    streamingmanager.cpp \
    recordingmanager.cpp \
    recordingdialog.cpp \
    hexconverterdialog.cpp \
    binaryvalidator.cpp

HEADERS  += mainwindow.h \
    filter/filter.h \
    filter/createfilterdialog.h \
    caninterface.h \
    packetconverter.h \
    chronomodel.h \
    uniquemodel.h \
    hexvalidator.h \
    streamingmanager.h \
    recordingmanager.h \
    recordingdialog.h \
    hexconverterdialog.h \
    binaryvalidator.h

FORMS    += mainwindow.ui \
    filter/createfilterdialog.ui \
    aboutdialog.ui \
    hexconverterdialog.ui

RESOURCES += \
    assets/assets.qrc

#muparser
unix|win32: LIBS += -L$$PWD/muparser/ -lmuparser

INCLUDEPATH += $$PWD/muparser
DEPENDPATH += $$PWD/muparser

DISTFILES +=
