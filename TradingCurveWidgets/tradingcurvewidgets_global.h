#ifndef TRADINGCURVEWIDGETS_GLOBAL_H
#define TRADINGCURVEWIDGETS_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QDebug>

#if defined(TRADINGCURVEWIDGETS_LIBRARY)
#  define TRADINGCURVEWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TRADINGCURVEWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#define FOR_TEST /* This Func is used for testing. */
#define DELETE_LATER /* depecrated func, should be deleted later. */

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#endif // TRADINGCURVEWIDGETS_GLOBAL_H
