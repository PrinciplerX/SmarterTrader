#ifndef REALTIMEDATAITEM_H
#define REALTIMEDATAITEM_H

#include <QObject>
#include "tradedata_global.h"
#include "tradedata.h"

class TRADEDATASHARED_EXPORT RealTimeDataItem : public AbstractRealTimeData
{
    Q_OBJECT
public:
    explicit RealTimeDataItem(const QString& symbol, QObject* parent = nullptr);
    virtual ~RealTimeDataItem();
    virtual const QuoteData* getQuoteData() const override;

    void update(QuoteData* newData);

private:
    QuoteData* data;
};

#endif // REALTIMEDATAITEM_H
