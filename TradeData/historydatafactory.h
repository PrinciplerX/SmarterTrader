#ifndef HISTORYDATAFACTORY_H
#define HISTORYDATAFACTORY_H

#include <QObject>
#include <QJsonArray>
#include <QUrl>
#include "tradedata_global.h"
#include "tradedata.h"
#include "dataproviderinfo.h"
#include "downloader.h"
#include "tradedataitem.h"

/*!
 * \brief ��ʷ������������һ��Ӧ�ó���ӵ��һ����ʷ�������������㹻�ˡ�
 */
class TRADEDATASHARED_EXPORT HistoryDataFactory : public QObject
{
    Q_OBJECT
public:
    explicit HistoryDataFactory(QObject *parent = 0);

    /*!
     * \brief ������ʷ���ݡ�
     * \param item ����ָ��Ҫ���ص����ݣ������ڽ������ݡ�
     */
    void request(TradeDataItem* item);

protected slots:
    /*!
     * \brief ���ڽ������洢QNetworkReply������յ������ݡ����ݱ���󣬽������ݴ洢����TradeDataItem��
     * ֪ͨӦ�ó��������Ѹ��µ���Ϣ��ͨ��TradeDataItem���źš�����
     * \warning ����ۺ���ֻ�ܺ�QNetworkReply����󶨡�
     */
    void onReplyFinished(bool isSucceed, int index, QByteArray data);

private:
    QMap<int, TradeDataItem*> replyMap;

    Downloader downloader;
};

#endif // HISTORYDATAFACTORY_H
