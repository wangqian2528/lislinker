QT      += core network websockets
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = hbfa.ico

TARGET   = ../../output/hbfa

SOURCES += \
        config.cpp \
        hbfaworker.cpp \
        main.cpp \
        tcpworker.cpp

HEADERS += \
    config.h \
    hbfaworker.h \
    tcpworker.h
