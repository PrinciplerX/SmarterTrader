#ifndef TRADEDATAHELPER_H
#define TRADEDATAHELPER_H

#include "tradedata_global.h"
#include "tradedata.h"

class TRADEDATASHARED_EXPORT TradeDataHelper
{
public:
    TradeDataHelper();

    static DateSeries getDateSeries(const QVector<TradeData>& data);

    static bool saveToCsv(const QString& fileName, const QVector<TradeData>& data);

    static bool loadFromCsv(const QString& fileName, QVector<TradeData>& data);

    /*!
     * \brief 将tick数据转化为1分钟周期的OHLC数据。
     * \param ticks tick数据，时刻精确到秒。
     * \return 1分钟周期的OHLC数据。
     */
    static TradeDataSeries fromTickData(const QVector<TickData>& ticks,
                                        DailyTradeHour tradeHour = DailyTradeHour());

    static void appendFromTickData(const QVector<TickData>& ticks, int parsedSize, QVector<TradeData>& output, DailyTradeHour tradeHour = DailyTradeHour());
};

#endif // TRADEDATAHELPER_H
