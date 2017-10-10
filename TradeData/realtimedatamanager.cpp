///===========================================================================///
/// \date 2016-08-10
/// \author 
/// \copyright Copyright ? 2015 - 2017 by YiChao Yang. All Rights Reserved.
///===========================================================================///

#include "realtimedatamanager.h"
#include "realtimedataitemproxy.h"

RealTimeDataManager* RealTimeDataManager::mInstance = nullptr;

RealTimeDataManager::RealTimeDataManager(QObject *parent):
    QObject(parent),
    timer(new QTimer(this))
{
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&factory, SIGNAL(replyFinished()), this, SIGNAL(dataUpdated()));
    timer->setInterval(3000);
    timer->start();
}

const AbstractRealTimeData *RealTimeDataManager::getRealTimeData(const QString &symbol)
{
    QMap<QString, RealTimeDataItem*>::iterator i = data.find(symbol);
    if(i != data.end()){
        return new RealTimeDataItemProxy(i.value());
    }else{
        RealTimeDataItem* item = new RealTimeDataItem(symbol);
        data.insert(symbol, item);
        return new RealTimeDataItemProxy(item);
    }
}

void RealTimeDataManager::update()
{
    factory.request(&data);
}

RealTimeDataManager &RealTimeDataManager::instance() {
    if(mInstance == nullptr)
        mInstance = new RealTimeDataManager();
    return *mInstance;
}
