#include "billboardmanger.h"

BillboardManger* BillboardManger::mInstance = nullptr;

BillboardManger &BillboardManger::instance()
{
    if(!mInstance)
        mInstance = new BillboardManger();
    return *mInstance;
}

AbstractBillboardData *BillboardManger::billboardData(const QString &startDate, const QString &endDate)
{
    BillboardDataItem* item = new BillboardDataItem(startDate, endDate);
    factory.request(item);
    return item;
}

BillboardManger::BillboardManger(QObject *parent) : QObject(parent)
{

}
