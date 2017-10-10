#ifndef DATAPROVIDERINFO_H
#define DATAPROVIDERINFO_H

#include "tradedata_global.h"
#include "tradedata.h"
#include "realtimedataitem.h"
#include <QString>
#include <QUrl>
#include <QMap>

/*!
 * \brief �ṩ��ʷ�����ṩ�̵Ľӿ���Ϣ��
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
     * \brief ��ȡ�ض�DataProvider��Url�ж���ʷ����ʱ�����ڲ����ı�ʾ��
     * \param provider �����ṩ�ߡ�
     * \param period ��ѯ�������ڡ�
     * \param argv ������ȡ��ѯ����Ĳ����
     * \return �����ѯ�ɹ�������true�����ִ����򷵻�false��
     */
    bool getPeriodArgv(DataProvider provider, DataPeriod period, QString& argv) const;

    bool getDataUrl(DataProvider provider, const HistoryDataIndex& index, QUrl& url) const;
    bool getDataUrl(DataProvider provider, DataPeriod period, const QString& symbol, QUrl& url) const;

    QString getStandardSymbol(DataProvider provider, const QString& symbol) const;
    QString getStandardSymbolList(DataProvider provider, const QStringList& symbols) const;
    QString getStandardSymbolList(DataProvider provider, const QMap<QString, RealTimeDataItem*>& data);

    /*!
     * \brief ��ȡʵʱ�������ݵĽӿ�url��
     * \param provider �����ṩ�ߡ�
     * \param symbol Ʒ�ִ��롣
     * \param url ʵʱ���ݽӿ�url��
     * \return �����ȡ�ɹ�������true�����ִ����򷵻�false��
     */
    bool getRealTimeDataUrl(DataProvider provider, const QString& symbol, QUrl& url) const;
    bool getRealTimeDataUrl(DataProvider provider, const QStringList& symbols, QUrl &url) const;

    /*!
     * \brief ��ȡtick���ݵĽӿ�url��
     * \param provider �����ṩ�ߡ�
     * \param symbol Ʒ�ִ��롣
     * \param date ����ȡtick�������ڡ�
     * \param url ���ڻ�ȡ����Ľӿ�url��
     * \return �����ȡ�ɹ�������true�����ִ����򷵻�false��
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
