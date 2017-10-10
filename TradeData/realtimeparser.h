#ifndef REALTIMEPARSER_H
#define REALTIMEPARSER_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "dataproviderinfo.h"
#include "QByteArray"

class TRADEDATASHARED_EXPORT RealTimeParser
{
public:
    /*!
     * \brief 解析数据接口返回的实时数据。
     * \param reply 数据接口返回数据，这里要求返回的实时数据的排序方式和数据存储容器（target）的遍历顺序是相同的。
     * \param provider 数据提供商。
     * \param target 数据存储容器。
     * \return
     */
    static bool parseReply(const QByteArray& reply, DataProviderInfo::DataProvider provider,
                           QMap<QString, RealTimeDataItem*>* target);
    static bool parseSinaReply(const QByteArray& reply, QMap<QString, RealTimeDataItem *> *target);
    static bool parseSinaReply(const QString& reply, QuoteData* target);

    static QString formatDoubleString(const QString &num, char format, int precision);
};

#endif // REALTIMEPARSER_H
