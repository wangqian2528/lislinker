QT      += core network serialport
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = diano.ico

TARGET   = ../../output/diano

SOURCES += \
    config.cpp \
    dianoworker.cpp \
        main.cpp \
    serialport.cpp

HEADERS += \
    config.h \
    dianoworker.h \
    serialport.h



