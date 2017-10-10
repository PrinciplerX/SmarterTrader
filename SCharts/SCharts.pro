requires(qtHaveModule(widgets))

load(qt_parts)

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += charts

include(../StGlobal.pri)
