QT      += core network
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = idexx.ico

TARGET   = ../../output/idexx

SOURCES += \
        analysis.cpp \
        config.cpp \
        main.cpp

HEADERS += \
    analysis.h \
    config.h
