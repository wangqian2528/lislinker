QT      += core gui widgets network serialport
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = app.ico

TARGET   = ../../output/app

SOURCES += \
    config.cpp \
    flatui.cpp \
    main.cpp \
    trayicon.cpp \
    window.cpp

HEADERS += \
    config.h \
    flatui.h \
    trayicon.h \
    window.h

FORMS += \
    window.ui

RESOURCES += \
    app.qrc \
    qss.qrc


