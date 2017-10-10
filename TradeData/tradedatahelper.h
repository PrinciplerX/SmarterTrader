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
     * \brief ��tick����ת��Ϊ1�������ڵ�OHLC���ݡ�
     * \param ticks tick���ݣ�ʱ�̾�ȷ���롣
     * \return 1�������ڵ�OHLC���ݡ�
     */
    static TradeDataSeries fromTickData(const QVector<TickData>& ticks,
                                        DailyTradeHour tradeHour = DailyTradeHour());

    static void appendFromTickData(const QVector<TickData>& ticks, int parsedSize, QVector<TradeData>& output, DailyTradeHour tradeHour = DailyTradeHour());
};

#endif // TRADEDATAHELPER_H
