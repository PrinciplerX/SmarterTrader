#include "tickdataitem.h"

TickDataItem::TickDataItem(const QString &symbol, const QString &date, QObject *parent):
    mSymbol(symbol),
    mDate(date)
{

}

int TickDataItem::size() const
{
    return mData.size();
}

const TickData &TickDataItem::at(int index) const
{
    return mData.at(index);
}

const QVector<TickData> &TickDataItem::data() const
{
    return mData;
}

void TickDataItem::setData(const QVector<TickData> &data)
{
    mData = data;
    emit dataLoaded();
}

void TickDataItem::append(const TickData &item)
{
    mData.append(item);
    emit dataAppended();
}

const QString &TickDataItem::getSymbol() const
{
    return mSymbol;
}

const QString &TickDataItem::getDate() const
{
    return mDate;
}
