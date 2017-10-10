#ifndef DATAPROVIDERINFO_H
#define DATAPROVIDERINFO_H

#include "tradedata_global.h"
#include "tradedata.h"
#include "realtimedataitem.h"
#include <QString>
#include <QUrl>
#include <QMap>

/*!
 * \brief 提供历史数据提供商的接口信息。
 */
class TRADEDATASHARED_EXPORT DataProviderInfo
{
public:
	enum DataProvider{
        YAHOO,
        SINA,
        TENCENT,
        IFENG
	};

    DataProviderInfo();

    static DataProviderInfo &getInstance();

    /*!
     * \brief 获取特定DataProvider的Url中对历史数据时间周期参数的表示。
     * \param provider 数据提供者。
     * \param period 查询数据周期。
     * \param argv 用来获取查询结果的参数项。
     * \return 如果查询成功，返回true，出现错误则返回false。
     */
    bool getPeriodArgv(DataProvider provider, DataPeriod period, QString& argv) const;

    bool getDataUrl(DataProvider provider, const HistoryDataIndex& index, QUrl& url) const;
    bool getDataUrl(DataProvider provider, DataPeriod period, const QString& symbol, QUrl& url) const;

    QString getStandardSymbol(DataProvider provider, const QString& symbol) const;
    QString getStandardSymbolList(DataProvider provider, const QStringList& symbols) const;
    QString getStandardSymbolList(DataProvider provider, const QMap<QString, RealTimeDataItem*>& data);

    /*!
     * \brief 获取实时交易数据的接口url。
     * \param provider 数据提供者。
     * \param symbol 品种代码。
     * \param url 实时数据接口url。
     * \return 如果获取成功，返回true；出现错误则返回false。
     */
    bool getRealTimeDataUrl(DataProvider provider, const QString& symbol, QUrl& url) const;
    bool getRealTimeDataUrl(DataProvider provider, const QStringList& symbols, QUrl &url) const;

    /*!
     * \brief 获取tick数据的接口url。
     * \param provider 数据提供者。
     * \param symbol 品种代码。
     * \param date 所获取tick所在日期。
     * \param url 用于获取输出的接口url。
     * \return 如果获取成功，返回true；出现错误则返回false。
     */
    bool getTickUrl(DataProvider provider, const QString& symbol, const QString& date, QUrl &url) const;

    bool getHistoryTickUrl(DataProvider provider, const QString& symbol, const QString& date, QUrl& url) const;

private:
    Q_DISABLE_COPY(DataProviderInfo)

    // UT = (url template)
    static const QString UT_HISTORY_IFENG_LONG;
    static const QString UT_HISTORY_IFENG_SHORT;
    static const QString UT_HISTORY_YAHOO;

    static const QString UT_REALTIME_SINA;
    static const QString UT_REALTIME_TENCENT;

    static const QString UT_TICK_SINA;
    static const QString UT_HISTORY_TICK_SINA;

    static DataProviderInfo sInstance;

    QMap<DataPeriod, QString> ifengPeriodArgv;
};

#endif // DATAPROVIDERINFO_H
