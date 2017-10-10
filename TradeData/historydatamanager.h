#ifndef HISTORYDATAMANAGER_H
#define HISTORYDATAMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "tradedata_global.h"
#include "tradedata.h"
#include "tradedataitem.h"
#include "tradedataitemproxy.h"
#include "historydatafactory.h"

/*!
 * \brief 用于在应用程序运行期间管理股票的历史数据，负责数据的获取，管理和释放。
 *
 * 获取数据的途径可能有很多种，如网络、数据库、以及特定格式的文件等。该类作为一个数据服务器，所有的历史数据请求都应
 * 该在这里处理。具体获取数据的途径对于调用者而言是不可见的。
 *
 * 历史数据不需要频繁请求， 理论上说在应用程序运行期间获取一次就可以了。在程序运行时，使用tick数据来更新历史数据。这
 * 样可以最大程度减少请求次数。
 *
 * 所支持对于数据的请求有两种方式：同步请求和异步请求。可以查询数据项是否存在，如果不存在可以按需获取。
 *
 * \todo 这里应该采取一些数据加载策略，可以提高运行期间的加载速度。
 * \date 2016-05-16
 */
class TRADEDATASHARED_EXPORT HistoryDataManager : public QObject
{
    Q_OBJECT
public:
    virtual ~HistoryDataManager();

    /*!
     * \todo 应该返回数据代理。
     */
    AbstractHistoryData* getHistoryData(const HistoryDataIndex& index);
    AbstractHistoryData* getHistoryData(const QString& symbol, DataPeriod period);

    AbstractHistoryData* fromTickData(const AbstractTickData* tickData, DailyTradeHour tradeHour);

    static HistoryDataManager& instance();

public slots:
    void update(const HistoryDataIndex& index, const TradeData& data);

protected:
    explicit HistoryDataManager(QObject *parent = 0);
    Q_DISABLE_COPY(HistoryDataManager)

private:
    //! 代码-数据映射表向量。每个向量元素对应一个周期。
    QVector<QMap<QString, TradeDataItem*>> data;
    HistoryDataFactory dataFactory;
    static HistoryDataManager* mInstance;
};

#endif // HISTORYDATAMANAGER_H
