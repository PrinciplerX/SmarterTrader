#ifndef TICKDATAITEM_H
#define TICKDATAITEM_H

#include "tradedata_global.h"
#include "tradedata.h"

class TRADEDATASHARED_EXPORT TickDataItem : public AbstractTickData
{
public:
    TickDataItem(const QString& symbol, const QString& date, QObject* parent = nullptr);

    virtual int size() const override;
    virtual const TickData& at(int index) const override;
    const TickData& operator[](int index) const { return at(index); }
    virtual const QVector<TickData>& data() const override;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // 更新数据
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void setData(const QVector<TickData>& data);
    void append(const TickData& item);

    virtual const QString& getSymbol() const override;
    virtual const QString& getDate() const override;

private:
    QString mDate;
    QString mSymbol;
    QVector<TickData> mData;
};

#endif // TICKDATAITEM_H
