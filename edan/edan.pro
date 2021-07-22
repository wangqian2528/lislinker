QT      += core network
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = edan.ico

TARGET   = ../../output/edan

SOURCES += \
        config.cpp \
        edanworker.cpp \
        main.cpp \
        netserver.cpp

HEADERS += \
        config.h \
        edanworker.h \
        netserver.h


