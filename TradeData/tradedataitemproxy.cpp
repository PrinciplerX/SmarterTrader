#include "tradedataitemproxy.h"

/*!
 * \brief ��TradeDataItem��Ϊ��QObject��parent���󣬲��������ݸ����¼�������ϵ��
 * \todo ��TradeDataItem�����destroyed()�¼�������Ӧ��
 */
TradeDataItemProxy::TradeDataItemProxy(TradeDataItem *item):
    AbstractHistoryData(item),
    dataItem(item)
{
    connect(item, SIGNAL(dataLoaded()), this, SIGNAL(dataLoaded()));
    connect(item, SIGNAL(dataAppended()), this, SIGNAL(dataAppended()));
    connect(item, SIGNAL(dataUpdated()), this, SIGNAL(dataUpdated()));
}

TradeDataItemProxy::~TradeDataItemProxy()
{
}

int TradeDataItemProxy::size() const {
    return dataItem->size();
}

const TradeData &TradeDataItemProxy::at(int index) const {
    return dataItem->at(index);
}

const QVector<TradeData> &TradeDataItemProxy::data() const
{
    return dataItem->data();
}

const QString &TradeDataItemProxy::getSymbol() const {
    return dataItem->getSymbol();
}

const QString &TradeDataItemProxy::getName() const {
    return dataItem->getName();
}

DataPeriod TradeDataItemProxy::getPeriod() const {
    return dataItem->getPeriod();
}

bool TradeDataItemProxy::attachIndicator(TechniqueIndicator *indicator)
{
    return dataItem->attachIndicator(indicator);
}

void TradeDataItemProxy::removeIndicator(TechniqueIndicator *indicator)
{
    dataItem->removeIndicator(indicator);
}

TechniqueIndicator *TradeDataItemProxy::findIndicator(const QString &name)
{
    return dataItem->findIndicator(name);
}

QVector<TechniqueIndicator *>& TradeDataItemProxy::indicators()
{
    return dataItem->indicators();
}
