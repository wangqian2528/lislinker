QT      += core network serialport
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = vb1.ico

TARGET   = ../../output/vb1

SOURCES += \
    config.cpp \
        main.cpp \
    serialport.cpp \
    vb1worker.cpp

HEADERS += \
    config.h \
    serialport.h \
    vb1worker.h


