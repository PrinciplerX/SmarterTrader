///===========================================================================///
/// \date 2016-12-22
/// \author 杨翌超
/// \copyright Copyright ? 2015 - 2017 by YiChao Yang. All Rights Reserved.
///===========================================================================///

#ifndef FUNCTIONALTRADEDATAITEM_H
#define FUNCTIONALTRADEDATAITEM_H

#include "tradedataitem.h"
#include "techniqueindicator.h"
#include "datagenetator.h"

class TRADEDATASHARED_EXPORT FunctionalTradeDataItem : public TradeDataItem
{
    Q_OBJECT
public:
    FunctionalTradeDataItem(TradeDataGenerator* generator, const HistoryDataIndex& dataIndex, const QString& itemName, QObject* parent = nullptr);
    FunctionalTradeDataItem(TradeDataGenerator* generator, const QString& symbol, DataPeriod period, const QString& itemName, QObject* parent = nullptr);
    virtual ~FunctionalTradeDataItem();

public slots:
    /*! \brief 更新下一周期的数据。*/
    void advance();

private:
    Q_DISABLE_COPY(FunctionalTradeDataItem)
    TradeDataGenerator* mGenerator;
};

#endif // FUNCTIONALTRADEDATAITEM_H
