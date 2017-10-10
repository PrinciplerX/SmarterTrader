#include "realtimetradedata.h"
#include "tradedatahelper.h"
#include "marketinfo.h"

RealTimeTradeData::RealTimeTradeData(AbstractTickData *tickData):
    TradeDataItem({tickData->getSymbol(), MINUTES_1}, tickData->getSymbol(), tickData)
{
    setTickData(tickData);
}

void RealTimeTradeData::setTickData(AbstractTickData *tickData)
{
    if(mTickData){
        disconnect(mTickData, SIGNAL(dataLoaded()), this, SLOT(onTickDataLoaded()));
        disconnect(mTickData, SIGNAL(dataAppended()), this, SLOT(onTickDataAppended()));
    }

    mTickData = tickData;

    connect(mTickData, SIGNAL(dataLoaded()), this, SLOT(onTickDataLoaded()));
    connect(mTickData, SIGNAL(dataAppended()), this, SLOT(onTickDataAppended()));

    onTickDataLoaded();
}

void RealTimeTradeData::onTickDataLoaded()
{
    if(mTickData && mTickData->size() != 0){
        setData(TradeDataHelper::fromTickData(mTickData->data(), MarketInfo::instance().tradeHour(MarketInfo::A_Shares)));
    }
}

void RealTimeTradeData::onTickDataAppended()
{

}
