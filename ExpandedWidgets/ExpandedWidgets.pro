#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T11:49:52
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = ExpandedWidgets
TEMPLATE = lib

CONFIG += c++11
CONFIG += shared
CONFIG += -debug-and-release

DEFINES += EXPANDEDWIDGETS_LIBRARY

SOURCES += \
    detailbutton.cpp \
    searchbox.cpp \
    titledlabel.cpp \
    verticallabel.cpp \
    maskwidget.cpp \
    roundprogressbar.cpp \
    drawerframe.cpp \
    detailedpanel.cpp \
    codeeditor.cpp \
    hierarchictabwidget.cpp \
    dockerframe.cpp


HEADERS +=\
    expandedwidgets_global.h \
    detailbutton.h \
    searchbox.h \
    titledlabel.h \
    verticallabel.h \
    maskwidget.h \
    roundprogressbar.h \
    drawerframe.h \
    detailedpanel.h \
    codeeditor.h \
    hierarchictabwidget.h \
    dockerframe.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

RESOURCES += \
    resource.qrc

DISTFILES +=

include(StGlobal.pri)
