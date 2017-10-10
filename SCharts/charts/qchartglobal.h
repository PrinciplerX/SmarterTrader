#ifndef SChartGlobal_H
#define SChartGlobal_H

#include <QtCore/QtGlobal>

#define SCHARTS_VERSION_STR QT_VERSION_STR
/*
   SCHARTS_VERSION is (major << 16) + (minor << 8) + patch.
*/
#define SCHARTS_VERSION QT_VERSION
/*
   can be used like #if (SCHARTS_VERSION >= SCHARTS_VERSION_CHECK(1, 1, 0))
*/
#define SCHARTS_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#if defined(SCHARTS_LIBRARY)
#  define SCHARTS_EXPORT Q_DECL_EXPORT
#else
#  define SCHARTS_EXPORT Q_DECL_IMPORT
#endif

#ifdef SCHARTS_STATICLIB
#  undef SCHARTS_EXPORT
#  undef Q_AUTOTEST_EXPORT
#  define SCHARTS_EXPORT
#  define Q_AUTOTEST_EXPORT
#endif

#define SCHARTS_NAMESPACE SCharts

#ifdef SCHARTS_NAMESPACE
#  define SCHARTS_BEGIN_NAMESPACE namespace SCHARTS_NAMESPACE {
#  define SCHARTS_END_NAMESPACE }
#  define SCHARTS_USE_NAMESPACE using namespace SCHARTS_NAMESPACE;
#else
#  define SCHARTS_BEGIN_NAMESPACE
#  define SCHARTS_END_NAMESPACE
#  define SCHARTS_USE_NAMESPACE
#endif

/*
    On Windows min and max conflict with standard macros
*/
#ifdef Q_OS_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#endif // SChartGlobal_H
