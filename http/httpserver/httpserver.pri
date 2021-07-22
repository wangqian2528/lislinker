INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core network

CONFIG += c++11

HEADERS += $$PWD/http_server.h \
           $$PWD/mongoose.h \

SOURCES += $$PWD/http_server.cpp \
           $$PWD/mongoose.c \

LIBS += -lAdvAPI32
