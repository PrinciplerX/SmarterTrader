#ifndef HISTORYDATAPARSER_H
#define HISTORYDATAPARSER_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "dataproviderinfo.h"
#include <QByteArray>

/*!
 * \brief 从某些数据源（如网络接口回复）解析历史数据，或者将历史数据转化为另一种格式（如csv）。
 */
class TRADEDATASHARED_EXPORT HistoryDataParser
{
public:
    /*!
     * \brief 将历史数据转化为csv格式。
     * \param series
     */
    static QString toCsv(const TradeDataSeries& series);

    /*!
     * \brief 解析从网络接口请求所得到的回复。不同的接口提供商的回复格式不同，所以解析方式也不同。
     * \param reply 从网络接口获取的数据信息。
     * \param provider 接口提供商。
     * \param output 用于接收解析结果。
     * \return 如果解析一切正常，则返回true，出现错误无法解析则返回false。
     */
    static bool parseReply(const QByteArray& reply, DataProviderInfo::DataProvider provider, TradeDataSeries& output);
    static bool parseIfengReply(const QByteArray& reply, TradeDataSeries& output);
    static bool parseYahooReply(const QByteArray& reply, TradeDataSeries& output);
    static bool parseYahooReply(const QString& reply, TradeDataSeries& output);

    static bool parseTdxFile(const QString path, TradeDataSeries& output);

private:
};

inline bool HistoryDataParser::parseYahooReply(const QByteArray &reply, TradeDataSeries &output){
    return parseYahooReply(QString(reply), output);
}

#endif // HISTORYDATAPARSER_H
