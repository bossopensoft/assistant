QT += testlib
QT += core gui network
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

include(../assistant/external/QtWebApp/QtWebApp/logging/logging.pri)
include(../assistant/external/QtWebApp/QtWebApp/httpserver/httpserver.pri)
include(../assistant/external/QtWebApp/QtWebApp/templateengine/templateengine.pri)

include(../assistant/src/controllers/controllers.pri)
#include(../assistant/src/view-models/view-models.pri)
include(../assistant/src/utility/utility.pri)


SOURCES +=  tst_test_entery.cpp \
    requirefactory.cpp \
    testcase_filehandler_test.cpp

HEADERS += \
    requirefactory.h \
    testcase_filehandler_test.h
