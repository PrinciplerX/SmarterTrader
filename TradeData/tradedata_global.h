#ifndef TRADEDATA_GLOBAL_H
#define TRADEDATA_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QDebug>

#if defined(TRADEDATA_LIBRARY)
#  define TRADEDATASHARED_EXPORT Q_DECL_EXPORT
#else
#  define TRADEDATASHARED_EXPORT Q_DECL_IMPORT
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#endif // TRADEDATA_GLOBAL_H
