QT      += core network
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = bc5000.ico

TARGET   = ../../output/bc5000

SOURCES += \
    bc5000worker.cpp \
    config.cpp \
    main.cpp \
    tcpworker.cpp

HEADERS += \
    bc5000worker.h \
    config.h \
    tcpworker.h



