#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T13:46:06
#
#-------------------------------------------------

QT       -= gui

TARGET = CtpClient
TEMPLATE = lib

DEFINES += CTPCLIENT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += ctpclient.cpp \
    ctprequester.cpp \
    ctpclientnotifier.cpp \
    stringhelper.cpp \
    ctpclientdata.cpp \
    ctpaccountmanager.cpp

HEADERS += ctpclient.h\
        ctpclient_global.h \
    ctprequester.h \
    ctpclientnotifier.h \
    stringhelper.h \
    ctpclientdata.h \
    ctpaccountmanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../include/
DEPENDPATH += $$PWD/../lib/
DEPENDPATH += $$PWD/../lib/ctp
LIBS += -L$$PWD/../lib/ctp/ -lthostmduserapi
LIBS += -L$$PWD/../lib/ctp/ -lthosttraderapi
LIBS += -L$$PWD/../lib/ -liconv

include(StGlobal.pri)
