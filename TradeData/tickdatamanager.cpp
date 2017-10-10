#include "tickdatamanager.h"
#include "realtimetradedata.h"

TickDataManager* TickDataManager::mInstance = nullptr;

TickDataManager::TickDataManager(QObject *parent) : QObject(parent)
{
}

AbstractTickData *TickDataManager::getTickData(const QString &symbol, const QString &date)
{
    QMap<QString, QMap<QString, TickDataItem*>>::iterator tickMapIterator = mData.find(symbol);
    if(tickMapIterator != mData.end()){
        QMap<QString, TickDataItem*>::iterator tickIterator = tickMapIterator->find(symbol);
        if(tickIterator != tickMapIterator->end()){
            return tickIterator.value();
        }else{
            TickDataItem* item = new TickDataItem(symbol, date, this);
            tickMapIterator->insert(date, item);
            dataFactory.request(item);
            return item;
        }
    }else{
        TickDataItem* item = new TickDataItem(symbol, date, this);
        QMap<QString, QMap<QString, TickDataItem*>>::iterator newIterator = mData.insert(symbol, QMap<QString, TickDataItem*>());
        newIterator->insert(date, item);
        dataFactory.request(item);
        return item;
    }
}

AbstractHistoryData *TickDataManager::getRealTimeTradeData(const QString &symbol, const QString &date)
{
    return new RealTimeTradeData(getTickData(symbol, date));
}

TickDataManager &TickDataManager::instance()
{
    if(!mInstance)
        mInstance = new TickDataManager();
    return *mInstance;
}
