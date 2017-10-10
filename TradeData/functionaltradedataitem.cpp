///===========================================================================///
/// \date 2016-12-22
/// \author 
/// \copyright Copyright ? 2015 - 2017 by YiChao Yang. All Rights Reserved.
///===========================================================================///

#include "functionaltradedataitem.h"

FunctionalTradeDataItem::FunctionalTradeDataItem(TradeDataGenerator* generator, const HistoryDataIndex &dataIndex, const QString &itemName, QObject *parent):
    TradeDataItem(dataIndex, itemName, parent)
{
    mGenerator = generator;
    mGenerator->setHistoryData(this);
}

FunctionalTradeDataItem::FunctionalTradeDataItem(TradeDataGenerator* generator, const QString &symbol, DataPeriod period, const QString &itemName, QObject *parent):
    TradeDataItem(symbol, period, itemName, parent)
{
    mGenerator = generator;
    mGenerator->setHistoryData(this);
}

FunctionalTradeDataItem::~FunctionalTradeDataItem()
{
    delete mGenerator;
}

void FunctionalTradeDataItem::advance()
{
    append(mGenerator->createNextData());
}
