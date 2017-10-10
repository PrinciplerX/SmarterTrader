#-------------------------------------------------
#
# Project created by QtCreator 2016-09-20T18:34:49
#
#-------------------------------------------------

TARGET = TradeCharts
TEMPLATE = lib

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += TRADECHARTS_LIBRARY

CONFIG += -debug-and-release

SOURCES += tradecharts.cpp \
    candlestick.cpp \
    candlestickanimation.cpp \
    candlestickdata.cpp \
    candlestickset.cpp \
    csplotanimation.cpp \
    csplotchartitem.cpp \
    csplotseries.cpp \
    fchart.cpp \
    fchartdataset.cpp \
    fchartlayout.cpp \
    fchartpresenter.cpp \
    fpanellayout.cpp \
    indexaxis.cpp \
    indexaxiselement.cpp \
    tradechartview.cpp \
    voidchartlayout.cpp

HEADERS += tradecharts.h\
        tradecharts_global.h \
    candlestick.h \
    candlestickanimation.h \
    candlestickdata.h \
    candlestickset.h \
    csplotanimation.h \
    csplotchartitem.h \
    csplotseries.h \
    fchart.h \
    fchartdataset.h \
    fchartlayout.h \
    fchartpresenter.h \
    fpanellayout.h \
    indexaxis.h \
    indexaxiselement.h \
    tradechartview.h \
    voidchartlayout.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../StGlobal.pri)

LIBS += -L$$SYSTEM_BIN -lSCharts

INCLUDEPATH += $$PWD/../SCharts/charts
DEPENDPATH += $$PWD/../SCharts/charts

