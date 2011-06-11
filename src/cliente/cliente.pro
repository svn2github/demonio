# -------------------------------------------------
# Project created by QtCreator 2010-05-30T21:33:58
# -------------------------------------------------
CONFIG += static
QT += network
TARGET = cliente
SOURCES += main.cpp \
    mainwindow.cpp \
    ventanaarchivos.cpp \
    ../../lib/source/Utilidades.cpp \
    ventanaescritorio.cpp \
    ventanaopciones.cpp \
    ventanawebcam.cpp \
    ventanapuertos.cpp
HEADERS += mainwindow.h \
    ventanaarchivos.h \
    ventanaescritorio.h \
    Forms/ventanaopciones.h \
    ventanaopciones.h \
    ventanawebcam.h \
    ventanapuertos.h
FORMS += Forms/mainwindow.ui \
    Forms/ventanaArchivos.ui \
    Forms/ventanaescritorio.ui \
    Forms/ventanaopciones.ui \
    Forms/ventanawebcam.ui \
    Forms/ventanaPuertos.ui

win32 {
QMAKE_LFLAGS += -static-libgcc
QTPLUGIN += qjpeg
RC_FILE = demonio.rc
}
TRANSLATIONS = cliente_en.ts \
               cliente_de.ts \
               cliente_fr.ts \
               cliente_it.ts \
               cliente_pt.ts
RESOURCES     = resources.qrc
