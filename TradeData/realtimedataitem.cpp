#include "realtimedataitem.h"

RealTimeDataItem::RealTimeDataItem(const QString &symbol, QObject *parent):
    AbstractRealTimeData(parent),
    data(new QuoteData)
{
    data->symbol = symbol;
}

RealTimeDataItem::~RealTimeDataItem()
{
    delete data;
}

const QuoteData *RealTimeDataItem::getQuoteData() const
{
    return data;
}

void RealTimeDataItem::update(QuoteData *newData)
{
    *data = *newData;
    emit dataUpdated();
}
