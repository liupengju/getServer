#-------------------------------------------------
#
# Project created by QtCreator 2016-09-19T00:16:42
#
#-------------------------------------------------

QT       += core gui xml axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = getServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    config.cpp \
    xlsaxobj.cpp \
    xmlhand.cpp \
    xmlanalysisthread.cpp \
    xmlanalysisdialog.cpp

HEADERS  += mainwindow.h \
    config.h \
    xlsaxobj.h \
    xmlhand.h \
    xmlanalysisthread.h \
    xmlanalysisdialog.h

FORMS    += mainwindow.ui \
    config.ui \
    xmlanalysisdialog.ui

RESOURCES += \
    src.qrc
