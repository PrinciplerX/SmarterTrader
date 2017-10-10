#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T00:23:53
#
#-------------------------------------------------

QT       += network
QT       += sql

QT       -= gui

TARGET = TradeData
TEMPLATE = lib

CONFIG += c++11
CONFIG += shared
CONFIG += -debug-and-release

DEFINES += TRADEDATA_LIBRARY

SOURCES += \
    yahooquotefactory.cpp \
    tickdatafactory.cpp \
    marketinfo.cpp \
    tickmodel.cpp \
    downloader.cpp \
    historydatafactory.cpp \
    dataproviderinfo.cpp \
    tradedatahelper.cpp \
    historydatamanager.cpp \
    historydataparser.cpp \
    tradedataitem.cpp \
    tradedataitemproxy.cpp \
    realtimedatamanager.cpp \
    realtimedataitem.cpp \
    realtimedataitemproxy.cpp \
    realtimedatafactory.cpp \
    realtimeparser.cpp \
    classifyingmanager.cpp \
    techniqueindicator.cpp \
    tickdatamanager.cpp \
    tickdataitem.cpp \
    realtimetradedata.cpp \
    newsmanager.cpp \
    newsfactory.cpp \
    billboardfactory.cpp \
    billboardmanger.cpp \
    functionalhistorydatamanager.cpp \
    functionaltradedataitem.cpp \
    functionalrealtimedataitem.cpp \
    functionalrealtimedatamanager.cpp \
    datagenetator.cpp \
    tradedataanalyser.cpp \
    tdxdata.cpp

HEADERS +=\
        tradedata_global.h \
    yahooquotefactory.h \
    tradedata.h \
    tickdatafactory.h \
    marketinfo.h \
    tickmodel.h \
    downloader.h \
    historydatafactory.h \
    dataproviderinfo.h \
    tradedatahelper.h \
    historydatamanager.h \
    historydataparser.h \
    realtimedatainterface.h \
    tradedataitem.h \
    tradedataitemproxy.h \
    realtimedatamanager.h \
    realtimedataitem.h \
    realtimedataitemproxy.h \
    realtimedatafactory.h \
    realtimeparser.h \
    classifyingmanager.h \
    techniqueindicator.h \
    tickdatamanager.h \
    tickdataitem.h \
    realtimetradedata.h \
    newsmanager.h \
    newsfactory.h \
    billboardfactory.h \
    billboardmanger.h \
    functionalhistorydatamanager.h \
    functionaltradedataitem.h \
    functionalrealtimedataitem.h \
    functionalrealtimedatamanager.h \
    datagenetator.h \
    tradedataanalyser.h \
    tdxdata.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../StGlobal.pri)
