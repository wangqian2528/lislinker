QT     += core network serialport

CONFIG += c++11

RC_ICONS = bc2600.ico

TARGET = ../../output/bc2600

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    MindrayWorker.cpp \
    SerialPortWorker.cpp \
    main.cpp

HEADERS += \
    GeneralProtocol.hpp \
    MindrayWorker.hpp \
    SerialPortWorker.hpp
