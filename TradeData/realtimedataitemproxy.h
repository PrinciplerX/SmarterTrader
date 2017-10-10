#ifndef REALTIMEDATAITEMPROXY_H
#define REALTIMEDATAITEMPROXY_H

#include "tradedata_global.h"
#include "tradedata.h"
#include "realtimedataitem.h"

class RealTimeDataItemProxy : public AbstractRealTimeData
{
    Q_OBJECT
public:
    explicit RealTimeDataItemProxy(RealTimeDataItem *item = 0);
    virtual const QuoteData* getQuoteData() const override;

private:
    RealTimeDataItem* dataItem;
};

#endif // REALTIMEDATAITEMPROXY_H
