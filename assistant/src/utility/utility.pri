INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/configmanager.h \
    $$PWD/inifilehandler.h \
    $$PWD/caconfigmanager.h \
    $$PWD/submoduleconfigmanager.h \
    $$PWD/iconfighandler.h \
    $$PWD/sysconfigmanager.h \
    $$PWD/utility.h \
    $$PWD/ziputils.h \
    $$PWD/installsubmodul.h \
    $$PWD/loggerutils.h

SOURCES += \
    $$PWD/configmanager.cpp \
    $$PWD/inifilehandler.cpp \
    $$PWD/caconfigmanager.cpp \
    $$PWD/submoduleconfigmanager.cpp \
    $$PWD/sysconfigmanager.cpp \
    $$PWD/ziputils.cpp \
    $$PWD/installsubmodul.cpp
