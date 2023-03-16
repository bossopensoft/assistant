QMAKE_LFLAGS += -pthread

SOURCES += \
        $$PWD/tinylogger.cpp

HEADERS += \
        $$PWD/blockingqueue.hpp \
        $$PWD/config.h \
        $$PWD/filelist.h \
        $$PWD/iloggerfilehandler.h \
        $$PWD/linuxloggerfile.h \
        $$PWD/stringutility.h \
        $$PWD/tinylogger.h \
        $$PWD/tracer.h
