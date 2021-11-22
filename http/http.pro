QT      += core network serialport
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = http.ico

TARGET   = ../../output/http

SOURCES += \
        config.cpp \
        main.cpp

HEADERS += \
    config.h

include(httpserver/httpserver.pri)
