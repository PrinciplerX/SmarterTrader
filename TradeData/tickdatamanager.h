#ifndef TICKDATAMANAGER_H
#define TICKDATAMANAGER_H

#include <QObject>
#include "tradedata.h"
#include "tickdatafactory.h"

/*!
 * \brief 分笔数据管理器。
 *
 * 分笔数据是最底层，最全面的交易数据。通过分笔数据可以计算得到不同周期的历史数据。当分笔数据更新时，应该
 * 将更新推送至不同周期的历史数据中（通过配合HistoryDataManager实现）。\todo 如何实现这一点？
 *
 * 对于获取某天一天的分时走势（即通过分笔数据计算得到的当天1分钟数据），应从TickDataManager中获取，而不应
 * 该从HistoryDataManager中获取。而对于连续的1分钟数据，则应该从HistoryDataManager中获取。
 */
class TRADEDATASHARED_EXPORT TickDataManager : public QObject
{
    Q_OBJECT
public:
    AbstractTickData* getTickData(const QString& symbol, const QString& date);
    AbstractHistoryData* getRealTimeTradeData(const QString& symbol, const QString& date);

    static TickDataManager& instance();

protected:
    explicit TickDataManager(QObject *parent = 0);
    Q_DISABLE_COPY(TickDataManager)

private:
    //! QMap<代码,QMap<日期,数据>>。
    QMap<QString, QMap<QString, TickDataItem*>> mData;
    TickDataFactory dataFactory;
    static TickDataManager* mInstance;
};

#endif // TICKDATAMANAGER_H
