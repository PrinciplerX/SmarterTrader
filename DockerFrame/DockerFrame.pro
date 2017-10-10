#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T09:51:21
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = DockerFrame
TEMPLATE = lib

DEFINES += DOCKERFRAME_LIBRARY

SOURCES += dockerframe.cpp \
    dockwidget.cpp \
    dockwidgettitle.cpp

HEADERS += dockerframe.h\
        dockerframe_global.h \
    dockwidget.h \
    dockwidgettitle.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
