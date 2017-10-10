#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T00:10:47
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = TradingCurveWidgets
TEMPLATE = lib

DEFINES += TRADINGCURVEWIDGETS_LIBRARY

CONFIG += c++11
CONFIG += shared
CONFIG += -debug-and-release

SOURCES += \
    traceplot.cpp \
    traceplotcanvas.cpp \
    tracescalewidget.cpp \
    tradingcurveplot.cpp \
    candlestick.cpp \
    bar.cpp \
    volume.cpp \
    tradingcurveplotpanel.cpp \
    simpleplot.cpp \
    simpleplotcanvas.cpp \
    macd.cpp \
    simplescalewidget.cpp \
    realtimeplotpanel.cpp \
    realtimeplot.cpp \
    abstractitemplot.cpp \
    titledplotaxis.cpp \
    realtimecurveplot.cpp \
    percentageaxis.cpp \
    percentagescaledraw.cpp \
    globalaverage.cpp \
    restingordertable.cpp \
    ticktable.cpp \
    splitterplotcanvas.cpp \
    realtimevolume.cpp \
    kdj.cpp \
    boll.cpp \
    plotablecurve.cpp \
    plotablecurvefactory.cpp \
    dmi.cpp \
    plotableitem.cpp \
    splitterscaledraw.cpp \
    splitterscalewidget.cpp \
    axisdata.cpp \
    abstractaxis.cpp \
    abstractplot.cpp \
    abstractplotlayout.cpp \
    abstracttraceplot.cpp \
    abstracttraceaxis.cpp \
    abstractitemaxis.cpp \
    realtimeitemaxis.cpp \
    tradingcurveitemaxis.cpp \
    splitterplot.cpp \
    abstractscaledraw.cpp \
    itemscaledraw.cpp \
    tradingcurvechart.cpp \
    rectangularaxisdraw.cpp \
    valuescaledraw.cpp \
    realtimecurvescaledraw.cpp \
    plottitlewidget.cpp \
    valueaxis.cpp \
    barchart.cpp \
    realtimecompositeplot.cpp \
    qwt_painter.cpp \
    qwt_null_paintdevice.cpp \
    qwt_date_scale_draw.cpp \
    qwt_text.cpp \
    qwt_text_engine.cpp \
    qwt_text_label.cpp \
    qwt_abstract_scale_draw.cpp \
    qwt_scale_draw.cpp \
    loghelper.cpp \
    qwt_date.cpp \
    qwt_date_scale_engine.cpp \
    qwt_scale_engine.cpp \
    qwt_scale_div.cpp \
    qwt_interval.cpp \
    qwt_math.cpp \
    qwt_color_map.cpp \
    qwt_scale_map.cpp \
    qwt_transform.cpp \
    qwt_clipper.cpp \
    qwt_point_polar.cpp \
    indicatorfactory.cpp \
    rsi.cpp \
    operation.cpp \
    tradedataoperations.cpp \
    asi.cpp \
    operationwrapperindicator.cpp \
    integeraxisdraw.cpp \
    voidaxisdraw.cpp \
    formulahighlighter.cpp \
    plotableobject.cpp

HEADERS +=\
        tradingcurvewidgets_global.h \
    traceplot.h \
    traceplotcanvas.h \
    tracescalewidget.h \
    tradingcurveplot.h \
    candlestick.h \
    bar.h \
    volume.h \
    tradingcurveplotpanel.h \
    simpleplot.h \
    simpleplotcanvas.h \
    macd.h \
    plotableitem.h \
    simplescalewidget.h \
    realtimeplotpanel.h \
    realtimeplot.h \
    abstractitemplot.h \
    titledplotaxis.h \
    realtimecurveplot.h \
    percentageaxis.h \
    percentagescaledraw.h \
    globalaverage.h \
    restingordertable.h \
    ticktable.h \
    splitterplotcanvas.h \
    realtimevolume.h \
    kdj.h \
    boll.h \
    plotablecurve.h \
    plotablecurvefactory.h \
    dmi.h \
    splitterscalewidget.h \
    splitterscaledraw.h \
    axisdata.h \
    abstractaxis.h \
    abstractplot.h \
    abstractplotlayout.h \
    abstracttraceplot.h \
    abstracttraceaxis.h \
    abstractitemaxis.h \
    realtimeitemaxis.h \
    tradingcurveitemaxis.h \
    splitterplot.h \
    abstractscaledraw.h \
    itemscaledraw.h \
    tradingcurvechart.h \
    rectangularaxisdraw.h \
    valuescaledraw.h \
    plottitlewidget.h \
    valueaxis.h \
    barchart.h \
    realtimecompositeplot.h \
    qwt_painter.h \
    qwt_null_paintdevice.h \
    qwt_date_scale_draw.h \
    qwt_text.h \
    qwt_text_engine.h \
    qwt_text_label.h \
    qwt_abstract_scale_draw.h \
    qwt_scale_draw.h \
    loghelper.h \
    qwt_date.h \
    qwt_date_scale_engine.h \
    qwt_scale_engine.h \
    qwt_scale_div.h \
    qwt_interval.h \
    qwt_math.h \
    qwt_color_map.h \
    qwt_transform.h \
    qwt_scale_map.h \
    qwt_clipper.h \
    qwt_point_polar.h \
    indicatorfactory.h \
    rsi.h \
    operation.h \
    tradedataoperations.h \
    asi.h \
    operationwrapperindicator.h \
    integeraxisdraw.h \
    voidaxisdraw.h \
    formulahighlighter.h \
    plotableobject.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../StGlobal.pri)

LIBS += -L$$SYSTEM_BIN -lTradeData

INCLUDEPATH += $$PWD/../TradeData
DEPENDPATH += $$PWD/../TradeData

LIBS += -L$$SYSTEM_BIN -lSCharts

INCLUDEPATH += $$PWD/../SCharts/charts
DEPENDPATH += $$PWD/../SCharts/charts
