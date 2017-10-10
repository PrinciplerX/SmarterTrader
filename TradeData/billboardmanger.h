#ifndef BILLBOARDMANGER_H
#define BILLBOARDMANGER_H

#include <QObject>
#include "tradedata.h"
#include "tradedata_global.h"
#include "billboardfactory.h"

class TRADEDATASHARED_EXPORT BillboardManger : public QObject
{
    Q_OBJECT
public:
    static BillboardManger& instance();
    AbstractBillboardData* billboardData(const QString& startDate, const QString& endDate);

signals:

private:
    explicit BillboardManger(QObject *parent = 0);

    Q_DISABLE_COPY(BillboardManger)

    BillboardFactory factory;
    static BillboardManger* mInstance;
};

#endif // BILLBOARDMANGER_H
