#ifndef REALTIMETRADEDATA_H
#define REALTIMETRADEDATA_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "tradedataitem.h"

/*!
 * \brief 分笔数据驱动的交易数据，主要包含某一交易日的分时走势数据。
 */
class TRADEDATASHARED_EXPORT RealTimeTradeData : public TradeDataItem
{
    Q_OBJECT
public:
    explicit RealTimeTradeData(AbstractTickData* tickData);
    void setTickData(AbstractTickData* tickData);

protected slots:
    void onTickDataLoaded();
    void onTickDataAppended();

private:
    AbstractTickData* mTickData = nullptr;

    /*! 已经处理的分笔数据项数。*/
    int parsedSize;
};

#endif // REALTIMETRADEDATA_H
