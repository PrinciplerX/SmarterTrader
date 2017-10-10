#ifndef REALTIMETRADEDATA_H
#define REALTIMETRADEDATA_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "tradedataitem.h"

/*!
 * \brief �ֱ����������Ľ������ݣ���Ҫ����ĳһ�����յķ�ʱ�������ݡ�
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

    /*! �Ѿ�����ķֱ�����������*/
    int parsedSize;
};

#endif // REALTIMETRADEDATA_H
