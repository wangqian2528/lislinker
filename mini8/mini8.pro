QT      += core
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = mini8.ico

TARGET   = ../../output/mini8

SOURCES += \
        config.cpp \
        main.cpp \
        mini8worker.cpp

HEADERS += \
    config.h \
    mini8worker.h
