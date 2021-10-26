QT      += core network serialport
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = h120.ico

TARGET   = ../../output/h120

SOURCES += \
        config.cpp \
        h120worker.cpp \
        main.cpp \
        serialport.cpp

HEADERS += \
    config.h \
    h120worker.h \
    serialport.h
