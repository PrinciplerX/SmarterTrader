#ifndef CTPCLIENT_GLOBAL_H
#define CTPCLIENT_GLOBAL_H

#include <QtCore/qglobal.h>

#include <ctp/ThostFtdcMdApi.h>
#include <ctp/ThostFtdcTraderApi.h>
#include <ctp/ThostFtdcUserApiDataType.h>
#include <ctp/ThostFtdcUserApiStruct.h>

#if defined(CTPCLIENT_LIBRARY)
#  define CTPCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CTPCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#endif // CTPCLIENT_GLOBAL_H
