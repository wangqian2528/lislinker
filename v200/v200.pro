QT      += core network
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = v200.ico

TARGET   = ../../output/v200

SOURCES += \
        config.cpp \
        main.cpp \
        tcpworker.cpp \
        v200worker.cpp

HEADERS += \
    config.h \
    tcpworker.h \
    v200worker.h



