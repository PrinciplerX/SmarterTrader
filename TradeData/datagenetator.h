#ifndef DATAGENETATOR_H
#define DATAGENETATOR_H

#include "tradedata.h"

class TRADEDATASHARED_EXPORT TradeDataGenerator
{
public:
    explicit TradeDataGenerator(AbstractHistoryData* data);
    void setHistoryData(AbstractHistoryData* data);
    virtual TradeData createNextData() = 0;
protected:
    const AbstractHistoryData* historyData() const;
private:
    AbstractHistoryData* mData;
};

class TRADEDATASHARED_EXPORT RandomDataGenerator : public TradeDataGenerator
{
public:
    explicit RandomDataGenerator(AbstractHistoryData* data);
    virtual TradeData createNextData() override;
};


#endif // DATAGENETATOR_H
