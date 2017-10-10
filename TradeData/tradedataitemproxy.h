#ifndef TRADEDATAITEMPROXY_H
#define TRADEDATAITEMPROXY_H

#include "tradedata.h"
#include "tradedata_global.h"
#include "tradedataitem.h"

/*!
 * \brief The TradeDataItemProxy class
 * \sa TradeDataItem
 */
class TRADEDATASHARED_EXPORT TradeDataItemProxy : public AbstractHistoryData
{
    Q_OBJECT
public:
    TradeDataItemProxy(TradeDataItem* item);
    virtual ~TradeDataItemProxy();

    virtual int size() const override;
    virtual const TradeData& at(int index) const override;
    virtual const QVector<TradeData>& data() const override;

    virtual const QString& getSymbol() const override;
    virtual const QString& getName() const override;
    virtual DataPeriod getPeriod() const override;

    virtual bool attachIndicator(TechniqueIndicator* indicator) override;
    virtual void removeIndicator(TechniqueIndicator* indicator) override;
    virtual TechniqueIndicator* findIndicator(const QString& name) override;
    virtual QVector<TechniqueIndicator*>& indicators();

private:
    Q_DISABLE_COPY(TradeDataItemProxy)
    TradeDataItem* dataItem;
};

#endif // TRADEDATAITEMPROXY_H
