#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T00:07:03
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SuperTrader
TEMPLATE = app

CONFIG += c++11

RC_FILE = resources.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    favoritelistmodel.cpp \
    newssitelistitem.cpp \
    newssitewidget.cpp \
    newswidget.cpp \
    indexquotepanel.cpp \
    marketpanel.cpp \
    realtimepanel.cpp \
    toolboxpanel.cpp \
    themesettingspanel.cpp \
    settingspanel.cpp \
    aboutwidget.cpp \
    datawidget.cpp \
    quotationlabel.cpp \
    favoritepanel.cpp \
    networksettingswidget.cpp \
    statisticpanel.cpp \
    historypanel.cpp \
    datamanager.cpp \
    datapanel.cpp \
    tradedatapanel.cpp \
    tickdatawidget.cpp \
    newstestwidget.cpp \
    uiconfigwidget.cpp \
    norwegianwoodstyle.cpp \
    billboardwidget.cpp \
    charttestpanel.cpp \
    testpanel.cpp \
    tcwtestwidget.cpp \
    accountpanel.cpp \
    stcharttestpanel.cpp

HEADERS  += mainwindow.h \
    favoritelistmodel.h \
    newssitelistitem.h \
    newssitewidget.h \
    newswidget.h \
    indexquotepanel.h \
    marketpanel.h \
    realtimepanel.h \
    toolboxpanel.h \
    themesettingspanel.h \
    mainframe_global.h \
    settingspanel.h \
    aboutwidget.h \
    datawidget.h \
    quotationlabel.h \
    favoritepanel.h \
    networksettingswidget.h \
    statisticpanel.h \
    historypanel.h \
    datamanager.h \
    datapanel.h \
    tradedatapanel.h \
    tickdatawidget.h \
    newstestwidget.h \
    uiconfigwidget.h \
    norwegianwoodstyle.h \
    billboardwidget.h \
    charttestpanel.h \
    testpanel.h \
    tcwtestwidget.h \
    accountpanel.h \
    stcharttestpanel.h

FORMS    +=

message($$OUT_PWD)

include(../StGlobal.pri)

LIBS += -L$$SYSTEM_BIN -lTradingCurveWidgets

INCLUDEPATH += $$PWD/../TradingCurveWidgets
DEPENDPATH += $$PWD/../TradingCurveWidgets

LIBS += -L$$SYSTEM_BIN -lTradeData

INCLUDEPATH += $$PWD/../TradeData
DEPENDPATH += $$PWD/../TradeData

LIBS += -L$$SYSTEM_BIN -lExpandedWidgets

INCLUDEPATH += $$PWD/../ExpandedWidgets
DEPENDPATH += $$PWD/../ExpandedWidgets

RESOURCES += \
    resource.qrc

LIBS += -L$$SYSTEM_BIN -lSCharts

INCLUDEPATH += $$PWD/../SCharts/charts
DEPENDPATH += $$PWD/../SCharts/charts

LIBS += -L$$SYSTEM_BIN -lTradeCharts

INCLUDEPATH += $$PWD/../TradeCharts
DEPENDPATH += $$PWD/../TradeCharts

LIBS += -L$$SYSTEM_BIN -lCtpClient

INCLUDEPATH += $$PWD/../include/
INCLUDEPATH += $$PWD/../CtpClient
DEPENDPATH += $$PWD/../lib/ctp
LIBS += -L$$PWD/../lib/ctp/ -lthostmduserapi
LIBS += -L$$PWD/../lib/ctp/ -lthosttraderapi
LIBS += -L$$PWD/../lib/ -liconv

DISTFILES +=
