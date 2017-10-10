#include "realtimedataitemproxy.h"

RealTimeDataItemProxy::RealTimeDataItemProxy(RealTimeDataItem *item) :
    AbstractRealTimeData(item),
    dataItem(item)
{
    connect(dataItem, SIGNAL(dataUpdated()), this, SIGNAL(dataUpdated()));
}

const QuoteData *RealTimeDataItemProxy::getQuoteData() const
{
    return dataItem->getQuoteData();
}
