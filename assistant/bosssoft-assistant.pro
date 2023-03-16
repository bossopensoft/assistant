#-------------------------------------------------
#
# Project created by QtCreator 2020-09-12T16:20:15
#
#-------------------------------------------------

QT       += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bosssoft-assistant
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


include(external/QtWebApp/QtWebApp/logging/logging.pri)
include(external/QtWebApp/QtWebApp/httpserver/httpserver.pri)
include(external/QtWebApp/QtWebApp/templateengine/templateengine.pri)

include(src/controllers/controllers.pri)
include(src/view-models/view-models.pri)
include(src/utility/utility.pri)
include(external/qtsingleapplication/qtsingleapplication.pri)
include(../tinylogger/tinylogger.pri)


SOURCES += \
        src/main.cpp



unix:{
    MOC_DIR = $$PWD/../build/$${TARGET}_build/moc
    OBJECTS_DIR = $$PWD/../build/$${TARGET}_build/object
    RCC_DIR = $$PWD/../build/$${TARGET}_build/rcc
    DESTDIR = $$PWD/../build
}

win32 {
    CONFIG(debug, debug|release){
        contains(QT_ARCH, i386) {
            LIBS += -L$$PWD/../lib/minizip/lib/win/x86/ -lminizipd -lAdvapi32 -lcrypt32 -lzlibstaticd
        }
        else {
            LIBS += -L$$PWD/../lib/minizip/lib/win/x64/ -lminizipd -lAdvapi32 -lcrypt32 -lzlibstaticd
        }
    }
    else {
        contains(QT_ARCH, i386) {
            LIBS += -L$$PWD/../lib/minizip/lib/win/x86/ -lminizip -lAdvapi32 -lcrypt32 -lzlibstatic
        }
        else {
            LIBS += -L$$PWD/../lib/minizip/lib/win/x64/ -lminizip -lAdvapi32 -lcrypt32 -lzlibstatic
        }
    }
}

unix {
    contains(QT_ARCH, arm64) {
        LIBS += -L$$PWD/../lib/minizip/lib/linux/arm64 -lminizip
    }
    contains(QT_ARCH, x86_64) {
        LIBS += -L$$PWD/../lib/minizip/lib/linux/x86 -lminizip
    }
    contains(QT_ARCH, mips64) {
        LIBS += -L$$PWD/../lib/minizip/lib/linux/mips -lminizip
    }
}

unix:{
LIBS += -lrt \
        -lz \
        -lbz2
}

INCLUDEPATH += $$PWD/../lib/minizip/include
DEPENDPATH += $$PWD/../lib/minizip/lib

EXTRA_LIB_DIR = $$PWD/../lib/.
# EXTRA_LIB_PACK_DIR = $$PWD/../build/DLL/3rdparty/

# QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$$EXTRA_LIB_DIR\" \"$$EXTRA_LIB_PACK_DIR\" $$escape_expand(\n\t)
