TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    MainFrame \
    TradingCurveWidgets \
    TradeData \
    ExpandedWidgets \
    SCharts \
    TradeCharts \ #\
    CtpClient
    # DockerFrame

CONFIG += c++11
CONFIG += -debug-and-release

include(StGlobal.pri)
