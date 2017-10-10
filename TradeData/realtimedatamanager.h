///===========================================================================///
/// \date 2016-08-10
/// \author 杨翌超
/// \copyright Copyright ? 2015 - 2017 by YiChao Yang. All Rights Reserved.
///===========================================================================///

#ifndef REALTIMEDATAMANAGER_H
#define REALTIMEDATAMANAGER_H

#include <QObject>
#include "tradedata_global.h"
#include "tradedata.h"
#include "realtimedataitem.h"
#include "realtimedatafactory.h"
#include <QTimer>

/*!
 * \brief 实时数据管理器。
 */
class TRADEDATASHARED_EXPORT RealTimeDataManager : QObject
{
    Q_OBJECT
public:
    static RealTimeDataManager& instance();

    const AbstractRealTimeData* getRealTimeData(const QString& symbol);

public slots:
    void update();

signals:
    void dataUpdated();

private:
    explicit RealTimeDataManager(QObject* parent = nullptr);

    RealTimeDataFactory factory;
    QMap<QString, RealTimeDataItem*> data;
    QTimer* timer;

    static RealTimeDataManager* mInstance;
};

#endif // REALTIMEDATAMANAGER_H
