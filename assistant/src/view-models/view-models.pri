INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/printingconfigdialog.h \
    $$PWD/systemtray.h \
    $$PWD/aboutdialog.h \
    $$PWD/syssettingdlg.h \
    $$PWD/designersettingmanager.h

SOURCES += \
    $$PWD/printingconfigdialog.cpp \
    $$PWD/systemtray.cpp \
    $$PWD/aboutdialog.cpp \
    $$PWD/syssettingdlg.cpp \
    $$PWD/designersettingmanager.cpp

RESOURCES += \
    $$PWD/systemtray.qrc

FORMS += \
    $$PWD/printingconfigdialog.ui \
    $$PWD/syssettingdlg.ui \
    $$PWD/aboutdialog.ui
