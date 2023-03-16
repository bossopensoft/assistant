QT += core network
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = daemon
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../assistant/external/QtWebApp/QtWebApp/httpserver/httpserver.pri)
include(controllers/controllers.pri)
include(qtsingleapplication/qtsingleapplication.pri)
include(../tinylogger/tinylogger.pri)

SOURCES += \
        main.cpp \
        programlauncher.cpp

unix:{
    MOC_DIR = $$PWD/../build/$${TARGET}_build/moc
    OBJECTS_DIR = $$PWD/../build/$${TARGET}_build/object
    RCC_DIR = $$PWD/../build/$${TARGET}_build/rcc
    DESTDIR = $$PWD/../build
}

HEADERS += \
    programlauncher.h\
    utility.h \
    loggerutils.h
