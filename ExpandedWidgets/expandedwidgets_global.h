#ifndef EXPANDEDWIDGETS_GLOBAL_H
#define EXPANDEDWIDGETS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EXPANDEDWIDGETS_LIBRARY)
#  define EXPANDEDWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EXPANDEDWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#endif // EXPANDEDWIDGETS_GLOBAL_H
