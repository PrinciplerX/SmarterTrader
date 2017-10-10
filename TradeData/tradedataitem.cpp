#include "tradedataitem.h"
#include "techniqueindicator.h"

TradeDataItem::TradeDataItem(const HistoryDataIndex &dataIndex, const QString &itemName, QObject *parent):
    AbstractHistoryData(parent),
    index(dataIndex),
    name(itemName)
{
}

TradeDataItem::TradeDataItem(const QString &symbol, DataPeriod period, const QString &itemName, QObject *parent):
    AbstractHistoryData(parent),
    name(itemName)
{
    index.period = period;
    index.symbol = symbol;
}

TradeDataItem::~TradeDataItem()
{

}

/*!
 * \todo 这很可能是一个耗费时间的操作，是否应该异步加载呢？
 */
void TradeDataItem::setData(const TradeDataSeries &data){
    this->mData = data;
    for(TechniqueIndicator* indicator : indicatorVector){
        indicator->onDataLoaded();
    }
    emit dataLoaded();
}

void TradeDataItem::append(const TradeData &item){
    mData.append(item);
    for(TechniqueIndicator* indicator : indicatorVector){
        indicator->onDataAppended();
    }
    emit dataAppended();
}

void TradeDataItem::update(const TradeData &item){
    mData[mData.size() - 1] = item;
    for(TechniqueIndicator* indicator : indicatorVector){
        indicator->onDataUpdated();
    }
    emit dataUpdated();
}

int TradeDataItem::size() const {
    return mData.size();
}

const TradeData &TradeDataItem::at(int index) const {
    return mData.at(index);
}

const QVector<TradeData> &TradeDataItem::data() const
{
    return mData;
}

const QString &TradeDataItem::getSymbol() const {
    return index.symbol;
}

const QString &TradeDataItem::getName() const {
    return name;
}

DataPeriod TradeDataItem::getPeriod() const {
    return index.period;
}

QVector<TechniqueIndicator *> &TradeDataItem::indicators()
{
    return indicatorVector;
}

void TradeDataItem::removeIndicator(TechniqueIndicator *indicator)
{
    if(!indicator) return;
    for(int i = 0; i < indicatorVector.size(); i++){
        if(indicatorVector[i]->indicatorName() == indicator->indicatorName()){
            indicatorVector.removeAt(i);
        }
    }
}

TechniqueIndicator *TradeDataItem::findIndicator(const QString &name)
{
    for(int i = 0; i < indicatorVector.size(); i++){
        if(indicatorVector[i]->indicatorName() == name){
            return indicatorVector[i];
        }
    }
    return nullptr;
}

bool TradeDataItem::attachIndicator(TechniqueIndicator *indicator)
{
    if(!indicator) return false;
    for(const TechniqueIndicator* item : indicatorVector){
        if(item->indicatorName() == indicator->indicatorName()){
            return false;
        }
    }
    indicatorVector.append(indicator);
    indicator->onDataLoaded();
    return true;
}
