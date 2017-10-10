#include "historydatamanager.h"
#include "tradedatahelper.h"

HistoryDataManager* HistoryDataManager::mInstance = nullptr;

HistoryDataManager::HistoryDataManager(QObject *parent) : QObject(parent)
{
    data.resize(DataPeriod::PERIOD_TYPE_COUNT);
}

AbstractHistoryData *HistoryDataManager::getHistoryData(const QString &symbol, DataPeriod period)
{
    QMap<QString, TradeDataItem*>& dataMap = data[period];
    QMap<QString, TradeDataItem*>::const_iterator i = dataMap.find(symbol);
    if(i != dataMap.end()){
        return new TradeDataItemProxy(i.value());
    }else{
        TradeDataItem* item = new TradeDataItem({ symbol, period }, QString("unknown"), this);
        dataMap.insert(symbol, item);
        dataFactory.request(item);
        return new TradeDataItemProxy(item);
    }
}

AbstractHistoryData *HistoryDataManager::fromTickData(const AbstractTickData *tickData, DailyTradeHour tradeHour)
{
    QMap<QString, TradeDataItem*>& dataMap = data[MINUTES_1];
    QMap<QString, TradeDataItem*>::const_iterator i = dataMap.find(tickData->getSymbol());
    if(i != dataMap.end()){
        return new TradeDataItemProxy(i.value());
    }else{
        TradeDataItem* item = new TradeDataItem({ tickData->getSymbol(), MINUTES_1 }, QString("Unknown"));
        item->setData(TradeDataHelper::fromTickData(tickData->data(), tradeHour));

        dataMap.insert(tickData->getSymbol(), item);
        return new TradeDataItemProxy(item);
    }
}

void HistoryDataManager::update(const HistoryDataIndex &index, const TradeData &data)
{
    QMap<QString, TradeDataItem*>& dataMap = this->data[index.period];
    QMap<QString, TradeDataItem*>::iterator i = dataMap.find(index.symbol);
    if(i != dataMap.end()){
        i.value()->update(data);
    }
}

HistoryDataManager &HistoryDataManager::instance()
{
    if(!mInstance)
        mInstance = new HistoryDataManager();
    return *mInstance;
}

HistoryDataManager::~HistoryDataManager()
{

}

AbstractHistoryData *HistoryDataManager::getHistoryData(const HistoryDataIndex &index) {
    return getHistoryData(index.symbol, index.period);
}
