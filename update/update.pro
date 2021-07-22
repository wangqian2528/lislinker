QT      +=  core gui widgets network
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = update.ico

TARGET   = ../../output/Linker

SOURCES += \
    config.cpp \
    flatui.cpp \
    main.cpp \
    updateworker.cpp \
    widget.cpp

HEADERS += \
    config.h \
    flatui.h \
    updateworker.h \
    widget.h

FORMS += \
    widget.ui

