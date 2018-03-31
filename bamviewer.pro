#-------------------------------------------------
#
# Project created by QtCreator 2018-01-21T16:57:25
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = bamviewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG+= c++14
# Set data example path

QMAKE_CXXFLAGS += -W -Wall -Wno-long-long -pedantic -Wno-variadic-macros

# Seqan configuration
DEFINES += "SEQAN_HAS_ZLIB=0"
DEFINES += "SEQAN_ENABLE_DEBUG=1"
DEFINES += "DSEQAN_ENABLE_TESTING=1"
DEFINES += "SEQAN_ASYNC_IO=0"
DEFINES += NDEBUG

INCLUDEPATH += "."

include(track/track.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        trackviewer.cpp

HEADERS += \
        mainwindow.h \
        trackviewer.h

FORMS += \
        mainwindow.ui


LIBS += -lws2_32


LIBS += -L$$PWD/../../../../../seqanextra/mingw/lib/ -lzlib


INCLUDEPATH += $$PWD/../../../../../seqanextra/mingw/include
DEPENDPATH += $$PWD/../../../../../seqanextra/mingw/include
