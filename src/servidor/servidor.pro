# -------------------------------------------------
# Project created by QtCreator 2010-05-30T18:46:25
# -------------------------------------------------
QT += network
TARGET = servidor
TEMPLATE = app

win32:SOURCES += main.cpp \
    mainwindow.cpp \
    ../../lib/source/Utilidades.cpp \
    ../../lib/source/webcamwin.cpp \
    ../../lib/source/tecladoyraton.cpp
win32:HEADERS += mainwindow.h \
    ../../lib/headers/Utilidades.h \
    ../../lib/headers/webcamwin.h \
    ../../lib/headers/tecladoyratonwin.h

unix:HEADERS += mainwindow.h \
    ../../lib/headers/Utilidades.h \
    ../../lib/headers/webcamlin.h \
    ../../lib/headers/tecladoyratonlin.h
unix:SOURCES += main.cpp \
    mainwindow.cpp \
    ../../lib/source/Utilidades.cpp \
    ../../lib/source/webcamlin.cpp \
    ../../lib/source/tecladoyratonlin.cpp


FORMS += Forms/mainwindow.ui
CONFIG += static
win32:LIBS += -lavicap32
win32:INCLUDEPATH += C:/MinGW32/include #ruta donde se encuentre el vfw.h
win32 {
QMAKE_LFLAGS += -static-libgcc -s -static
QMAKE_LFLAGS_CONSOLE = -Wl,-subsystem,windows
QTPLUGIN += qjpeg
}
CONFIG += qtestlib
CONFIG -= console
