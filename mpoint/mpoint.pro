QT      += core network serialport
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = mpoint.ico

TARGET   = ../../output/mpoint

SOURCES += \
        config.cpp \
        main.cpp \
        mpworker.cpp \
        serialport.cpp

HEADERS += \
    config.h \
    mpworker.h \
    serialport.h
