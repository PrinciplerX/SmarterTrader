#ifndef TICKDATAFACTORY_H
#define TICKDATAFACTORY_H

#include <QVector>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QFile>
#include "tradedata.h"
#include "tradedata_global.h"
#include "downloader.h"
#include "dataproviderinfo.h"
#include "tickdataitem.h"

/*!
 * \brief The TickDataFactory class
 */
class TRADEDATASHARED_EXPORT TickDataFactory : public QObject
{
    Q_OBJECT
public:
    explicit TickDataFactory(QObject *parent = 0);
    static QVector<TickData> parse(const QString& fileData);
    static QVector<TickData> parseFile(const QString& fileName);
    static bool parseSinaReply(const QByteArray& reply, QVector<TickData>& output);

    /*!
     * \brief ������ʷ���ݡ�
     * \param item ����ָ��Ҫ���ص����ݣ������ڽ������ݡ�
     */
    void request(TickDataItem* item);

protected slots:
    /*!
     * \brief ���ڽ������洢QNetworkReply������յ������ݡ����ݱ���󣬽������ݴ洢����TradeDataItem��
     * ֪ͨӦ�ó��������Ѹ��µ���Ϣ��ͨ��TradeDataItem���źš�����
     * \warning ����ۺ���ֻ�ܺ�QNetworkReply����󶨡�
     */
    void onReplyFinished(bool isSucceed, int index, QByteArray data);

private:
    QMap<int, TickDataItem*> replyMap;

    QVector<QMap<QString, TickDataItem*>> mData;
    Downloader downloader;
};

#endif // TICKDATAFACTORY_H
