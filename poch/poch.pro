QT      += core network serialport
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = poch.ico

TARGET   = ../../output/poch

SOURCES += \
    config.cpp \
        main.cpp \
    pochworker.cpp \
    serialport.cpp

HEADERS += \
    config.h \
    dataframe.h \
    pochworker.h \
    serialport.h



