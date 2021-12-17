QT      += core gui widgets
CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app

RC_ICONS = app.ico

TARGET   = ../../output/app

SOURCES += \
    config.cpp \
    main.cpp \
    trayicon.cpp \
    window.cpp

HEADERS += \
    config.h \
    trayicon.h \
    window.h

FORMS +=

RESOURCES += \
    app.qrc \
    qss.qrc


